#include <QtCore/QStringList>

#include "child/node.h"
#include "child/exception.h"

namespace Child {

bool Node::_initialized = Node::initRoot();

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
    static NodePtr _root(new Node(NodePtr()));
    return _root;
}

bool Node::initRoot() {
    root()->addChild("Node", root());
    return true;
}

NodePtr Node::find(const QString &name) { return context()->child(name); }

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
    _setChild(name, NodePtr());
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
    if(_parents) foreach(const Node *parent, _parents->keys()) parents.append(NodePtr(parent));
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

void Node::throwRuntimeException(const QString &message, const QString &file,
                               const int line, const QString &function) {
    throw RuntimeExceptionPtr(new RuntimeException(message, file, line, function));
}

void Node::throwNullPointerException(const QString &message, const QString &file,
                               const int line, const QString &function) {
    throw NullPointerExceptionPtr(new NullPointerException(message, file, line, function));
}

void Node::throwTypecastException(const QString &message, const QString &file,
                               const int line, const QString &function) {
    throw TypecastExceptionPtr(new TypecastException(message, file, line, function));
}

const QString Node::inspect() const {
    QString str;
    str = hexMemoryAddress();
    str.append(": [");
    str.append(QStringList(children().keys()).join(", "));
    str.append("]");
    return str;
}

} // namespace Child

/*
namespace Child {
    CHILD_DEFINITION(Node, Node, Node);

    Node::~Node() {
        unsetOrigin();
        removeAllForks();
        removeAllExtensions();
        removeAllExtendedNodes();
        removeAllParents();
        removeAllDirectChildren();
        nodeCount()--;
    }

    void Node::initRoot() {
        // TODO
    }

    Node *Node::setOrigin(Node *node) {
        checkNode(node);
        if(_origin == node) { return this; }
        if(_origin) { _origin->_forks->removeOne(this); }
        _origin = node;
        if(!node->_forks) { node->_forks = new NodeList; }
        node->_forks->append(this);
        return this;
    }

    bool Node::directOriginIs(Node *node) const {
        checkNode(node);
        return _origin == node;
    }

    void Node::removeAllForks() {
        if(!_forks) { return; }
        foreach(Node *node, *_forks) {
            delete node;
        }
        delete _forks;
        _forks = NULL;
    }

    bool Node::hasExtension(Node *node) const {
        checkNode(node);
        return _extensions ? _extensions->contains(node) : false;
    }

    void Node::addExtension(Node *node) {
        checkNode(node);
        if(!_extensions) { _extensions = new NodeList; }
        if(_extensions->contains(node)) throw DuplicateException("cannont add an extension which is already there");
        _extensions->append(node);
        if(!node->_extendedNodes) { node->_extendedNodes = new NodeList; }
        node->_extendedNodes->append(this);
    }

    void Node::prependExtension(Node *node) {
        checkNode(node);
        if(!_extensions) { _extensions = new NodeList; }
        if(_extensions->contains(node)) throw DuplicateException("cannont add an extension which is already there");
        _extensions->prepend(node);
        if(!node->_extendedNodes) { node->_extendedNodes = new NodeList; }
        node->_extendedNodes->append(this);
    }

    void Node::removeExtension(Node *node) {
        checkNode(node);
        if(!_extensions || !_extensions->contains(node)) throw NotFoundException("cannot remove an extension which is not there");
        _extensions->removeOne(node);
        node->_extendedNodes->removeOne(this);
    }

    void Node::removeAllExtensions() {
        if(!_extensions) { return; }
        foreach(Node *node, *_extensions) {
            node->_extendedNodes->removeOne(this);
        }
        delete _extensions;
        _extensions = NULL;
    }

    void Node::removeAllExtendedNodes() {
        if(!_extendedNodes) { return; }
        foreach(Node *node, *_extendedNodes) {
            delete node;
        }
        delete _extendedNodes;
        _extendedNodes = NULL;
    }

    const NodeMultiHash Node::parents() const {
        NodeMultiHash hash;
        if(_parents) {
            foreach(NamedNode parent, *_parents) {
                hash.insert(parent.name, parent.node);
            }
        }
        return hash;
    }

    bool Node::hasDirectParent(Node *node) const {
        checkNode(node);
        return node->_children && !node->_children->key(const_cast<Node *>(this)).isEmpty();
    }

    void Node::addParent(const QString &name, Node *node) {
        NamedNode namedNode = NamedNode(name, node);
        if(!_parents) { _parents = new NamedNodeSet; }
        if(_parents->contains(namedNode)) throw DuplicateException("duplicate node/child name");
        if(!node->_children) { node->_children = new NodeHash; }
        if(node->_children->contains(name)) throw DuplicateException("duplicate child name in parent");
        _parents->insert(namedNode);
        node->_children->insert(name, this);
    }

    void Node::removeParent(const QString &name, Node *node) {
        NamedNode namedNode = NamedNode(name, node);
        if(!_parents || !_parents->contains(namedNode)) throw NotFoundException("child not found");
        _parents->remove(namedNode);
        node->_children->remove(name);
        deleteIfOrphan();
    }

    void Node::removeAllParents() {
        if(!_parents) { return; }
        foreach(NamedNode parent, *_parents) {
            parent.node->_children->remove(parent.name);
        }
        delete _parents;
        _parents = NULL;
    }

    bool Node::hasDirectChild(const QString &name) const {
        checkName(name);
        return _children && _children->contains(name);
    }

    Node *Node::directChild(const QString &name) const {
        checkName(name);
        if(!_children) throw NotFoundException("child not found");
        Node *value = _children->value(name);
        if(!value) throw NotFoundException("child not found");
        return value;
    }

    Node *Node::addDirectChild(const QString &name, Node *node) {
        checkName(name);
        checkNode(node);
        if(_children && _children->contains(name)) throw DuplicateException("duplicate child name");
        node->addParent(name, this);
        return node;
    }

    Node *Node::setDirectChild(const QString &name, Node *node) {
        checkName(name);
        checkNode(node);
        if(!_children || !_children->contains(name)) throw NotFoundException("child not found");
        Node *currentChild = directChild(name);
        if(currentChild != node) {
            currentChild->removeParent(name, this);
            node->addParent(name, this);
        }
        return node;
    }

    Node *Node::addOrSetDirectChild(const QString &name, Node *node) {
        checkName(name);
        checkNode(node);
        if(!_children || !_children->contains(name)) {
            node->addParent(name, this);
        } else {
            Node *currentChild = directChild(name);
            if(currentChild != node) {
                currentChild->removeParent(name, this);
                node->addParent(name, this);
            }
        }
        return node;
    }

    void Node::removeDirectChild(const QString &name) {
        checkName(name);
        if(!_children) throw NotFoundException("child not found");
        Node *node = _children->value(name);
        if(!node) throw NotFoundException("child not found");
        node->removeParent(name, this);
    }

    void Node::removeAllDirectChildren() {
        if(!_children) { return; }
        NodeHashIterator i(*_children);
        while(i.hasNext()) {
            i.next();
            i.value()->_parents->remove(NamedNode(i.key(), this));
            i.value()->deleteIfOrphan();
        }
        delete _children;
        _children = NULL;
    }

    Node *Node::_getOrSetChild(const QString &name, Node *setValue, bool returnThisIfFound) {
        NodeSet nodeSeen;
        NodeQueue nodeQueue;
        NodeQueue parentQueue;
        parentQueue.enqueue(this);
        QHash<Node *, NamedNode> parentTree;
        Node *child = NULL;
        while(!parentQueue.isEmpty()) {
            Node *parent = parentQueue.dequeue();
            nodeQueue.enqueue(parent);
            while(!nodeQueue.isEmpty()) {
                Node *node = nodeQueue.dequeue();
//                p(node->inspect());
                if(node->hasDirectChild(name)) { child = node->directChild(name); break; }
                if(node->_origin) {
                    if(!nodeSeen.contains(node->_origin)) {
                        nodeSeen.insert(node->_origin);
                        nodeQueue.enqueue(node->_origin);
                    }
                }
                if(node->_extensions) {
                    foreach(Node *n, *(node->_extensions)) {
                        if(!nodeSeen.contains(n)) {
                            nodeSeen.insert(n);
                            nodeQueue.enqueue(n);
                        }
                    }
                }
                if(node->_parents) {
                    foreach(NamedNode par, *(node->_parents)) {
                        if(!parentTree.contains(par.node)) {
                            parentTree.insert(par.node, NamedNode(par.name, parent));
                            parentQueue.enqueue(par.node);
                        }
                    }
                }
            }
            if(child) {
                if(returnThisIfFound) { return this; }
                NamedNodeList parentPath;
                while(parent != this) {
                    NamedNode par(parentTree.value(parent));
                    parentPath.prepend(NamedNode(par.name, parent));
                    parent = par.node;
                }
                bool mustVirtualize = false;
                foreach(NamedNode par, parentPath) {
                    if(!mustVirtualize && (!parent->_parents || !parent->_parents->contains(par))) mustVirtualize = true;
                    if(mustVirtualize) {
//                        p("Forking parent " + par.node->inspect());
                        Node *virtualParent = par.node->fork()->setIsVirtual(true);
                        parent->addParent(par.name, virtualParent);
                        parent = virtualParent;
                    } else {
                        parent = par.node;
                    }
                }
                if(mustVirtualize || !child->_parents || !child->_parents->contains(NamedNode(name, parent))) {
                    if(setValue) {
                        child = setValue;
                    } else {
                        // p("Cloning child " + child->inspect());
                        child = child->fork()->setIsVirtual(true);
                    }
                    child->addParent(name, parent);
                } else {
                    if(setValue) {
                        if(child != setValue) {
                            child->removeParent(name, parent);
                            child = setValue;
                            child->addParent(name, parent);
                        }
                    }
                }
                NativeMethod *meth = dynamic_cast<NativeMethod *>(child);
                if(meth && meth->method()) {
                    child = (parent->*meth->method())();
                }
                return child;
            }
        }
        return NULL;
    }

    bool Node::hasChild(const QString &name) const {
        checkName(name);
        return const_cast<Node *>(this)->_getOrSetChild(name, NULL, true);
    }

    Node *Node::child(const QString &name) const {
        checkName(name);
        if(hasDirectChild(name)) return directChild(name);
        Node *child = const_cast<Node *>(this)->_getOrSetChild(name);
        if(!child) throw NotFoundException("child not found");
        return child;
    }

    Node *Node::setChild(const QString &name, Node *node) {
        checkName(name);
        checkNode(node);
        Node *child = _getOrSetChild(name, node);
        if(!child) throw NotFoundException("child not found");
        return child;
    }

    const QString Node::inspect() const {
        QString str;
        str = hexMemoryAddress();
        str.append(": [");
        str.append(QStringList(parents().keys()).join(", "));
        str.append("] => [");
        str.append(QStringList(children().keys()).join(", "));
        str.append("]");
        return str;
    }
}
*/
