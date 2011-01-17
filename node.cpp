#include <QtCore/QStringList>

#include "node.h"
#include "node/exception.h"
#include "node/nativemethod.h"
#include "node/object/boolean.h"
#include "node/object/number.h"
#include "node/object/text.h"
#include "node/object/message.h"
#include "node/object/block.h"
#include "node/object/method.h"
#include "node/object/property.h"

CHILD_BEGIN

const bool Node::isInitialized = Node::root();

Node::~Node() {
    if(_extensions) delete _extensions;
    if(_children) {
        foreach(Node *child, *_children) if(child) child->_removeParent(this);
        delete _children;
    }
    if(_parents) delete _parents;
}

Node *Node::root() {
    static Node *_root = NULL;
    if(!_root) {
        _root = new Node(NULL);
        _root->setOrigin(_root);
        _root->addChild("Node", _root);
        _root->declare("Node");
        _root->initRoot();
    }
    return _root;
}

void Node::initRoot() {
    pushContext(this);

    CHILD_ADD_NATIVE_METHOD(Node, self);

    Property::root();

    Property *originProperty = CHILD_PROPERTY();
    originProperty->CHILD_ADD_NATIVE_METHOD(Node, origin_get, get);
    originProperty->CHILD_ADD_NATIVE_METHOD(Node, origin_set, set);
    addChild("origin", originProperty);

    Property *extensionsProperty = CHILD_PROPERTY();
    extensionsProperty->CHILD_ADD_NATIVE_METHOD(Node, extensions_get, get);
    addChild("extensions", extensionsProperty);

    CHILD_ADD_NATIVE_METHOD(Node, fork);
    CHILD_ADD_NATIVE_METHOD(Node, init);

    CHILD_ADD_NATIVE_METHOD(Node, define, :=);
    CHILD_ADD_NATIVE_METHOD(Node, assign, =);

    CHILD_ADD_NATIVE_METHOD(Node, parent);
    CHILD_ADD_NATIVE_METHOD(Node, parent_qm, parent?);

    CHILD_ADD_NATIVE_METHOD(Node, or, ||);
    CHILD_ADD_NATIVE_METHOD(Node, and, &&);
    CHILD_ADD_NATIVE_METHOD(Node, not, !);

    CHILD_ADD_NATIVE_METHOD(Node, or_assign, ||=);
    CHILD_ADD_NATIVE_METHOD(Node, and_assign, &&=);

    CHILD_ADD_NATIVE_METHOD(Node, equal_to, ==);
    CHILD_ADD_NATIVE_METHOD(Node, different_from, !=);
    CHILD_ADD_NATIVE_METHOD(Node, assert, ?:);
    CHILD_ADD_NATIVE_METHOD(Node, print);
    CHILD_ADD_NATIVE_METHOD(Node, inspect);

    CHILD_ADD_NATIVE_METHOD(Node, memory_address);
}

const QString Node::nodeName() const {
    return child("__name__")->toString();
}

void Node::setNodeName(const QString &name) {
    addOrSetChild("__name__", CHILD_TEXT(name));
}

const QString Node::nodePath() const {
    QString path;
    const Node *cur = this;
    const Node *par;
    while((par = cur->parent()) != cur) {
        path.prepend(par->nodeName().toLower() + "/");
        cur = par;
    }
    return path;
}

void Node::declare(const QString &name) const {
    roots().append(Root(constCast(this), name));
}

void Node::initFork() {
    setIsAutoRunnable(origin()->isAutoRunnable());
}

CHILD_DEFINE_NATIVE_METHOD(Node, fork) {
    CHILD_FIND_LAST_MESSAGE;
    Node *node = fork();
    if(node->hasChild("init")) {
        Message* initMessage = message->fork();
        initMessage->setName("init");
        initMessage->run(node);
    }
    return node;
}

CHILD_DEFINE_NATIVE_METHOD(Node, init) {
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *nextPrimitive = primitive->next();
    if(nextPrimitive) {
        nextPrimitive->run(this);
        throw Primitive::Skip(this);
    }
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Node, self) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Node, origin_get) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return parent()->origin();
}

CHILD_DEFINE_NATIVE_METHOD(Node, origin_set) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    parent()->setOrigin(message->runFirstInput());
    return this;
}

void Node::setOrigin(Node *node) {
    CHILD_CHECK_POINTER(node);
    _origin = node;
}

bool Node::isOriginatingFrom(Node *orig) const {
    orig = orig->real();
    const Node *node = real();
    while(node != orig) {
        if(node->origin() == node) // Node::root reached?
            return false;
        node = node->origin();
    }
    return true;
}

Node *Node::real() {
    Node *node = this;
    while(node->isVirtual()) node = node->origin();
    return node;
}

void Node::addExtension(Node *node) {
    CHILD_CHECK_POINTER(node);
    if(!_extensions) { _extensions = new QList<Node *>; }
    if(hasExtension(node)) CHILD_THROW(DuplicateException, "cannot add an extension which is already there");
    _extensions->append(node);
}

void Node::prependExtension(Node *node)  {
    CHILD_CHECK_POINTER(node);
    if(!_extensions) { _extensions = new QList<Node *>; }
    if(hasExtension(node)) CHILD_THROW(DuplicateException, "cannot add an extension which is already there");
    _extensions->prepend(node);
}

void Node::removeExtension(Node *node)  {
    CHILD_CHECK_POINTER(node);
    if(!hasExtension(node)) CHILD_THROW(NotFoundException, "cannot remove an extension which is not there");
    _extensions->removeOne(node);
}

void Node::removeAllExtensions() {
    _extensions->clear();
}

bool Node::hasExtension(Node *node) const {
    CHILD_CHECK_POINTER(node);
    return _extensions && _extensions->contains(node);
}

QList<Node *> Node::extensions() const {
    return _extensions ? *_extensions : QList<Node *>();
}

CHILD_DEFINE_NATIVE_METHOD(Node, extensions_get) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    VirtualList *value = VirtualList::dynamicCast(hasDirectChild("cached_value"));
    if(!value) {
        value = CHILD_VIRTUAL_LIST(&parent()->_extensions);
        value->setAreDuplicateValuesNotAllowed(true);
        addOrSetChild("cached_value", value);
    }
    return value;
}

Node *Node::child(const QString &name) const {
    Node *node = hasChild(name);
    if(!node) CHILD_THROW(NotFoundException, "child not found");
    return node;
}

Node *Node::addChild(const QString &name, Node *value) {
    CHILD_CHECK_POINTER(value);
    if(hasChild(name, false)) CHILD_THROW(DuplicateException, "child with same name is already there");
    _setChild(name, value);
    return value;
}

Node *Node::setChild(const QString &name, Node *value, bool addOrSetMode) {
    CHILD_CHECK_POINTER(value);
    Node *parent = this;
    bool isDirect;
    if(Node *current = hasChild(name, !addOrSetMode, &parent, false, &isDirect)) {
        if(isDirect) {
            if(current == value) return value;
            current->_removeParent(parent);
        }
    } else if(!addOrSetMode) CHILD_THROW(NotFoundException, "child not found");
    parent->_setChild(name, value);
    return value;
}

void Node::_setChild(const QString &name, Node *value) {
    if(!_children) _children = new QHash<QString, Node *>;
    _children->insert(name, value);
    if(value) value->_addParent(this);
}

Node *Node::defineOrAssign(bool isDefine) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(2);
    Message *msg = Message::dynamicCast(message->firstInput()->value()->value());
    if(!msg) CHILD_THROW(ArgumentException, "left-hand side is not a message");
    if(msg->name() == "[]") {
        message = message->fork();
        message->setName(isDefine ? "[]:=" : "[]=");
        if(msg->hasAnInput())
            message->inputs()->set(0, msg->firstInput());
        else
            message->inputs()->remove(0);
        return message->run(this);
    }
    Node *value;
    Block *block = Block::dynamicCast(message->secondInput()->value()->value());
    if(block) { // if rhs is a block, we have a method definition shorthand
        value = CHILD_METHOD(NULL, NULL, block);
    } else // rhs is not a block
        value = message->runSecondInput();
    Property *property = NULL;
    if(!isDefine && (property = Property::dynamicCast(hasChild(msg->name()))))
        CHILD_MESSAGE("set", value)->run(property);
    else
        setChild(msg->name(), value, isDefine);
    if(isDefine) value->hasBeenDefined(msg);
    return value;
}

void Node::hasBeenDefined(Message *message) {
    if(!message->name().isEmpty() && message->name().at(0).isUpper()) {
        setNodeName(message->name());
        if(message->hasAnInput()) {
            Method *method = Method::dynamicCast(hasDirectChild("init"));
            if(!method) {
                method = CHILD_METHOD();
                method->setIsAutoRunnable(true);
                method->setNodeName("init");
                addOrSetChild("init", method);
            }
            method->setInputs(message->inputs());
        }
    }
}

void Node::removeChild(const QString &name) {
    bool isDirect;
    if(Node *current = hasChild(name, false, NULL, false, &isDirect)) {
        if(isDirect) current->_removeParent(this);
    } else CHILD_THROW(NotFoundException, "child not found");
    _setChild(name, NULL);
}

Node *Node::hasChild(const QString &name, bool searchInParents, Node **parentPtr, bool autoFork, bool *isDirectPtr) const {
    Node *node = hasChildInSelfOrOrigins(name, autoFork, isDirectPtr);
    if(searchInParents) {
        if(node) {
            if(parentPtr) *parentPtr = constCast(this);
        } else if(_parents)
            foreach(Node *parent, _parents->keys()) {
                if(parent != this) { // for Node::root which is child of itself
                    node = parent->hasChild(name, searchInParents, parentPtr, autoFork, isDirectPtr);
                    if(node) break;
                }
            }
    }
    return node;
}

Node *Node::hasChildInSelfOrOrigins(const QString &name, bool autoFork, bool *isDirectPtr) const {
    bool isRemoved;
    Node *node = hasDirectChild(name, &isRemoved);
    bool isDirect = node || isRemoved;
    if(!isDirect) {
        if(origin() != this) node = origin()->hasChildInSelfOrOrigins(name);
        if(!node && _extensions) {
            foreach(Node *extension, *_extensions) {
                node = extension->hasChildInSelfOrOrigins(name);
                if(node) break;
            }
        }
        if(node && autoFork) {
            node = node->fork();
            node->setIsVirtual(true);
            constCast(this)->_setChild(name, node);
        }
    }
    if(isDirectPtr) *isDirectPtr = isDirect;
    return node;
}

void Node::_addParent(Node *parent) const {
    HugeUnsignedInteger count = 0;
    if(_parents)
        count = _parents->value(parent);
    else
        _parents = new QHash<Node *, HugeUnsignedInteger>;
    _parents->insert(parent, count + 1);
}

void Node::_removeParent(Node *parent) const {
    if(!_parents) CHILD_THROW(NotFoundException, "parent not found");
    HugeUnsignedInteger count = _parents->value(parent) - 1;
    if(count > 0)
        _parents->insert(parent, count);
    else if(count == 0)
        _parents->remove(parent);
    else
        CHILD_THROW(NotFoundException, "parent not found");
}

QHash<QString, Node *> Node::children() const {
    QHash<QString, Node *> children;
    if(_children) {
        QHashIterator<QString, Node *> i(*_children);
        while(i.hasNext()) if(i.next().value()) children.insert(i.key(), i.value());
    }
    return children;
}

QList<Node *> Node::parents() const {
    QList<Node *> parents;
    if(_parents) foreach(Node *parent, _parents->keys()) parents.append(parent);
    return parents;
}

Node *Node::parent() const {
    if(!hasOneParent()) CHILD_THROW_RUNTIME_EXCEPTION("zero or more than one parent found");
    return _parents->keys().first();
}

CHILD_DEFINE_NATIVE_METHOD(Node, parent) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return parent();
}

bool Node::hasOneParent() const {
    return _parents && _parents->size() == 1;
}

CHILD_DEFINE_NATIVE_METHOD(Node, parent_qm) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_BOOLEAN(hasOneParent());
}

Node *Node::receive(Primitive *primitive) {
    return primitive->run(this);
}

CHILD_DEFINE_NATIVE_METHOD(Node, or) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_BOOLEAN(toBool() || message->runFirstInput()->toBool());
}

CHILD_DEFINE_NATIVE_METHOD(Node, and) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_BOOLEAN(toBool() && message->runFirstInput()->toBool());
}

CHILD_DEFINE_NATIVE_METHOD(Node, not) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_BOOLEAN(!toBool());
}

CHILD_DEFINE_NATIVE_METHOD(Node, or_assign) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(2);
    Node *lhs = message->runFirstInput();
    if(!lhs->toBool())
        return CHILD_MESSAGE("=", message->firstInput(), message->secondInput())->run();
    else
        return lhs;
}

CHILD_DEFINE_NATIVE_METHOD(Node, and_assign) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(2);
    Node *lhs = message->runFirstInput();
    if(lhs->toBool())
        return CHILD_MESSAGE("=", message->firstInput(), message->secondInput())->run();
    else
        return lhs;
}

CHILD_DEFINE_NATIVE_METHOD(Node, equal_to) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_BOOLEAN(isEqualTo(message->runFirstInput()));
}

CHILD_DEFINE_NATIVE_METHOD(Node, different_from) {
    CHILD_FIND_LAST_MESSAGE;
    return CHILD_BOOLEAN(!Boolean::cast(CHILD_MESSAGE("==", message->inputs(false))->run(this))->value());
}

CHILD_DEFINE_NATIVE_METHOD(Node, assert) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    CHILD_FIND_LAST_PRIMITIVE;
    Primitive *nextPrimitive = primitive->next();
    if(!nextPrimitive) CHILD_THROW(InterpreterException, "missing code after '?:' statement");
    Node *result = nextPrimitive->run();
    if(!result->toBool()) CHILD_THROW(AssertionException, "assertion failed");
    passedAssertionCount()++;
    throw Primitive::Skip(result);
}

CHILD_DEFINE_NATIVE_METHOD(Node, print) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    print();
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Node, inspect) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    inspect();
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Node, memory_address) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_NUMBER(memoryAddress());
}

QString Node::toString(bool debug, short level) const {
    Q_UNUSED(debug);
    Q_UNUSED(level);
    return QString("%1:%2: [%3]").arg(nodeName()).arg(hexMemoryAddress(), QStringList(children().keys()).join(", "));
}

CHILD_END
