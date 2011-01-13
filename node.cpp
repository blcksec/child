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
        _root->initRoot();
        registerRoot(_root, "Node");
    }
    return _root;
}

void Node::initRoot() {
    pushContext(this);

    CHILD_NATIVE_METHOD_ADD(Node, self);

    Property::root();

    Property *originProperty = CHILD_PROPERTY();
    originProperty->CHILD_NATIVE_METHOD_ADD(Node, origin_get, get);
    originProperty->CHILD_NATIVE_METHOD_ADD(Node, origin_set, set);
    addChild("origin", originProperty);

    CHILD_NATIVE_METHOD_ADD(Node, fork);
    CHILD_NATIVE_METHOD_ADD(Node, define, :=);
    CHILD_NATIVE_METHOD_ADD(Node, assign, =);

    CHILD_NATIVE_METHOD_ADD(Node, parent);
    CHILD_NATIVE_METHOD_ADD(Node, parent_qm, parent?);

    CHILD_NATIVE_METHOD_ADD(Node, or, ||);
    CHILD_NATIVE_METHOD_ADD(Node, and, &&);
    CHILD_NATIVE_METHOD_ADD(Node, not, !);

    CHILD_NATIVE_METHOD_ADD(Node, or_assign, ||=);
    CHILD_NATIVE_METHOD_ADD(Node, and_assign, &&=);

    CHILD_NATIVE_METHOD_ADD(Node, equal_to, ==);
    CHILD_NATIVE_METHOD_ADD(Node, different_from, !=);
    CHILD_NATIVE_METHOD_ADD(Node, assert, ?:);
    CHILD_NATIVE_METHOD_ADD(Node, print);
    CHILD_NATIVE_METHOD_ADD(Node, inspect);

    CHILD_NATIVE_METHOD_ADD(Node, memory_address);
}

const QString Node::className() const {
    return child("__name__")->toString();
}

void Node::setClassName(const QString &name) {
    addOrSetChild("__name__", CHILD_TEXT(name));
}

const QString Node::classPath() const {
    QString path;
    const Node *cur = this;
    const Node *par;
    while((par = cur->parent()) != cur) {
        path.prepend(par->className().toLower() + "/");
        cur = par;
    }
    return path;
}

CHILD_NATIVE_METHOD_DEFINE(Node, self) {
    CHILD_CHECK_INPUT_SIZE(0);
    return this;
}

CHILD_NATIVE_METHOD_DEFINE(Node, fork) {
    CHILD_CHECK_INPUT_SIZE(0);
    Node *node = fork();
    if(node->hasChild("init")) {
        Message* initMessage = message->fork();
        initMessage->setName("init");
        initMessage->run(node);
    }
    return node;
}

CHILD_NATIVE_METHOD_DEFINE(Node, origin_get) {
    CHILD_CHECK_INPUT_SIZE(0);
    return parent()->origin();
}

CHILD_NATIVE_METHOD_DEFINE(Node, origin_set) {
    CHILD_CHECK_INPUT_SIZE(1);
    parent()->setOrigin(message->runFirstInput());
    return this;
}

void Node::setOrigin(Node *node) {
    CHILD_CHECK_POINTER(node);
    _origin = node;
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

Node *Node::child(const QString &name) const {
    Node *node = hasChild(name);
    if(!node) CHILD_THROW(NotFoundException, "child not found");
    return node;
}

void Node::addChild(const QString &name, Node *value) {
    CHILD_CHECK_POINTER(value);
    if(hasChild(name, false)) CHILD_THROW(DuplicateException, "child with same name is already there");
    _setChild(name, value);
}

void Node::setChild(const QString &name, Node *value, bool addOrSetMode) {
    CHILD_CHECK_POINTER(value);
    Node *parent = this;
    bool isDirect;
    if(Node *current = hasChild(name, !addOrSetMode, &parent, false, &isDirect)) {
        if(isDirect) {
            if(current == value) return;
            current->_removeParent(parent);
        }
    } else if(!addOrSetMode) CHILD_THROW(NotFoundException, "child not found");
    parent->_setChild(name, value);
}

void Node::_setChild(const QString &name, Node *value) {
    if(!_children) _children = new QHash<QString, Node *>;
    _children->insert(name, value);
    if(value) value->_addParent(this);
}

Node *Node::defineOrAssign(Message *message, bool isDefine) {
    CHILD_CHECK_INPUT_SIZE(2);
    Primitive *primitive = message->firstInput()->value();
    Message *msg = Message::dynamicCast(primitive->value());
    if(!msg) CHILD_THROW(ArgumentException, "left-hand side is not a message");
    Node *value;
    Block *block = Block::dynamicCast(message->secondInput()->value()->value());
    if(block) // if rhs is a block, we have a method definition shorthand
        value = CHILD_METHOD(NULL, NULL, block);
    else // rhs is not a block
        value = message->runSecondInput();
    Property *property = NULL;
    if(!isDefine && (property = Property::dynamicCast(hasChild(msg->name()))))
        CHILD_MESSAGE("set", value)->run(property);
    else
        setChild(msg->name(), value, isDefine);
    value->hasBeenAssigned(msg);
    return value;
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

CHILD_NATIVE_METHOD_DEFINE(Node, parent) {
    CHILD_CHECK_INPUT_SIZE(0);
    return parent();
}

bool Node::hasOneParent() const {
    return _parents && _parents->size() == 1;
}

CHILD_NATIVE_METHOD_DEFINE(Node, parent_qm) {
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_BOOLEAN(hasOneParent());
}

Node *Node::receive(Primitive *primitive) {
    return primitive->run(this);
}

Node *Node::run(Node *receiver, Message *message, Primitive *code) {
    Q_UNUSED(receiver);
    Q_UNUSED(code);
    if(message->inputs(false)) {
        Message *forkMessage = message->fork();
        forkMessage->setName("fork");
        return forkMessage->run(this);
    } else
        return this;
}

CHILD_NATIVE_METHOD_DEFINE(Node, or) {
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_BOOLEAN(toBool() || message->runFirstInput()->toBool());
}

CHILD_NATIVE_METHOD_DEFINE(Node, and) {
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_BOOLEAN(toBool() && message->runFirstInput()->toBool());
}

CHILD_NATIVE_METHOD_DEFINE(Node, not) {
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_BOOLEAN(!toBool());
}

CHILD_NATIVE_METHOD_DEFINE(Node, or_assign) {
    CHILD_CHECK_INPUT_SIZE(2);
    Node *lhs = message->runFirstInput();
    if(!lhs->toBool())
        return CHILD_MESSAGE("=", message->firstInput(), message->secondInput())->run();
    else
        return lhs;
}

CHILD_NATIVE_METHOD_DEFINE(Node, and_assign) {
    CHILD_CHECK_INPUT_SIZE(2);
    Node *lhs = message->runFirstInput();
    if(lhs->toBool())
        return CHILD_MESSAGE("=", message->firstInput(), message->secondInput())->run();
    else
        return lhs;
}

CHILD_NATIVE_METHOD_DEFINE(Node, equal_to) {
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_BOOLEAN(isEqualTo(message->runFirstInput()));
}

CHILD_NATIVE_METHOD_DEFINE(Node, different_from) {
    return CHILD_BOOLEAN(!Boolean::cast(CHILD_MESSAGE("==", message->inputs(false))->run(this))->value());
}

CHILD_NATIVE_METHOD_DEFINE(Node, assert) {
    CHILD_CHECK_INPUT_SIZE(0);
    if(!toBool()) CHILD_THROW(AssertionException, "assertion failed");
    passedAssertionCount()++;
    return this;
}

CHILD_NATIVE_METHOD_DEFINE(Node, print) {
    CHILD_CHECK_INPUT_SIZE(0);
    print();
    return this;
}

CHILD_NATIVE_METHOD_DEFINE(Node, inspect) {
    CHILD_CHECK_INPUT_SIZE(0);
    inspect();
    return this;
}

CHILD_NATIVE_METHOD_DEFINE(Node, memory_address) {
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_NUMBER(memoryAddress());
}

QString Node::toString(bool debug, short level) const {
    Q_UNUSED(debug);
    Q_UNUSED(level);
    return QString("%1:%2: [%3]").arg(className()).arg(hexMemoryAddress(), QStringList(children().keys()).join(", "));
}

CHILD_END
