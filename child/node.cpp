#include <QtCore/QStringList>

#include "child/node.h"
#include "child/exception.h"

CHILD_BEGIN

const bool Node::isInitialized = Node::root().isNotNull();

Node::~Node() {
    if(_extensions) delete _extensions;
    if(_children) {
        foreach(NodePtr child, *_children) if(child) child->_removeParent(this);
        delete _children;
    }
    if(_parents) delete _parents;
    nodeCount()--;
}

NodePtr &Node::root() {
    static NodePtr _root;
    if(!_root) {
        _root = new Node(NULL);
        initRoot();
    }
    return _root;
}

void Node::setOrigin(const NodePtr &node) {
    CHILD_CHECK_NODE_PTR(node);
    _origin = node;
}

void Node::addExtension(const NodePtr &node) {
    CHILD_CHECK_NODE_PTR(node);
    if(!_extensions) { _extensions = new QList<NodePtr>; }
    if(hasExtension(node)) CHILD_THROW(DuplicateException, "cannot add an extension which is already there");
    _extensions->append(node);
}

void Node::prependExtension(const NodePtr &node)  {
    CHILD_CHECK_NODE_PTR(node);
    if(!_extensions) { _extensions = new QList<NodePtr>; }
    if(hasExtension(node)) CHILD_THROW(DuplicateException, "cannot add an extension which is already there");
    _extensions->prepend(node);
}

void Node::removeExtension(const NodePtr &node)  {
    CHILD_CHECK_NODE_PTR(node);
    if(!hasExtension(node)) CHILD_THROW(NotFoundException, "cannot remove an extension which is not there");
    _extensions->removeOne(node);
}

void Node::removeAllExtensions() {
    _extensions->clear();
}

bool Node::hasExtension(const NodePtr &node) const {
    CHILD_CHECK_NODE_PTR(node); return _extensions && _extensions->contains(node);
}

const NodePtr Node::child(const QString &name) const {
    NodePtr node = hasChild(name);
    if(!node) CHILD_THROW(NotFoundException, "child not found");
    return node;
}

const NodePtr Node::addChild(const QString &name, const NodePtr &value) {
    CHILD_CHECK_NODE_PTR(value);
    if(hasChild(name, false)) CHILD_THROW(DuplicateException, "child with same name is already there");
    _setChild(name, value);
    return value;
}

const NodePtr Node::setChild(const QString &name, const NodePtr &value) {
    CHILD_CHECK_NODE_PTR(value);
    bool isDirect;
    if(NodePtr current = hasChild(name, true, false, &isDirect)) {
        if(isDirect) {
            if(current == value) return value;
            current->_removeParent(this);
        }
    } else CHILD_THROW(NotFoundException, "child not found");
    _setChild(name, value);
    return value;
}

void Node::removeChild(const QString &name) {
    bool isDirect;
    if(NodePtr current = hasChild(name, true, false, &isDirect)) {
        if(isDirect) current->_removeParent(this);
    } else CHILD_THROW(NotFoundException, "child not found");
    _setChild(name, NULL);
}

const NodePtr Node::hasChild(const QString &name, bool searchInParents,
                             bool forkChildFoundInFirstOrigin, bool *isDirectPtr) const {
    bool isRemoved;
    NodePtr node = hasDirectChild(name, &isRemoved);
    bool isDirect = !node.isNull() || isRemoved;
    if(!isDirect)
        if(origin() && (node = origin()->hasChild(name, searchInParents)))
            if(forkChildFoundInFirstOrigin) {
                node = node->fork();
                const_cast<Node *>(this)->_setChild(name, node);
            }
    if(isDirectPtr) *isDirectPtr = isDirect;
    return node;
}

const QHash<QString, NodePtr> Node::children() const {
    QHash<QString, NodePtr> children;
    if(_children) {
        QHashIterator<QString, NodePtr> i(*_children);
        while(i.hasNext()) if(i.next().value()) children.insert(i.key(), i.value());
    }
    return children;
}

const QList<NodePtr> Node::parents() const {
    QList<NodePtr> parents;
    if(_parents) foreach(const Node *parent, _parents->keys()) parents.append(parent);
    return parents;
}

void Node::_setChild(const QString &name, const NodePtr &value) {
    if(!_children) _children = new QHash<QString, NodePtr>;
    _children->insert(name, value);
    if(value) value->_addParent(this);
}

void Node::_addParent(const Node *parent) const {
    HugeUnsignedInteger count = 0;
    if(_parents)
        count = _parents->value(parent);
    else
        _parents = new QHash<const Node *, HugeUnsignedInteger>;
    _parents->insert(parent, count + 1);
}

void Node::_removeParent(const Node *parent) const {
    if(!_parents) CHILD_THROW(NotFoundException, "parent not found");
    HugeUnsignedInteger count = _parents->value(parent) - 1;
    if(count > 0)
        _parents->insert(parent, count);
    else if(count == 0)
        _parents->remove(parent);
    else
        CHILD_THROW(NotFoundException, "parent not found");
}

const QString Node::toString(bool debug) const {
    Q_UNUSED(debug);
    return QString("%1: [%2]").arg(hexMemoryAddress(), QStringList(children().keys()).join(", "));
}

CHILD_END
