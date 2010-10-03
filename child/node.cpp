#include "child/node.h"
#include "child/nativemethod.h"
#include "child/text.h"

namespace Child {
    NamedNode::NamedNode(const QString &t, Node *const n) : name(t), node(n) {
        if(t.isEmpty()) { throw ArgumentException("Empty name passed to NamedNode constructor()"); }
        if(!n) { throw NullPointerException("NULL node passed to NamedNode constructor()"); }
    }

    long long int Node::_nodeCount = 0;
    Node *Node::_root = Node::root();

    Node::~Node() {
        unsetOrigin();
        removeAllForks();
        removeAllExtensions();
        removeAllExtendedNodes();
        removeAllParents();
        removeAllDirectChildren();
        _nodeCount--;
    }

    Node *Node::root() {
        if(!_root) {
            _root = new Node;
            _root->addParent("Node", _root);
        }
        return(_root);
    }

    void Node::setOrigin(Node *node) {
        if(!node) { throw NullPointerException("NULL value passed to Node::setBase()"); }
        if(_origin == node) { return; }
        if(_origin) { _origin->_forks->removeOne(this); }
        _origin = node;
        if(!node->_forks) { node->_forks = new NodeList; }
        node->_forks->append(this);
    }

    bool Node::directOriginIs(Node *node) const {
        if(!node) { throw NullPointerException("NULL value passed to Node::isForkedFrom()"); }
        return(_origin == node);
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
        if(!node) { throw NullPointerException("NULL value passed to Node::hasExtension()"); }
        return(_extensions ? _extensions->contains(node) : false);
    }

    void Node::addExtension(Node *node) {
        if(!node) { throw NullPointerException("NULL value passed to Node::addExtension()"); }
        if(!_extensions) { _extensions = new NodeList; }
        if(_extensions->contains(node)) { throw DuplicateException("Duplicate node passed to Node::addExtension()"); }
        _extensions->append(node);
        if(!node->_extendedNodes) { node->_extendedNodes = new NodeList; }
        node->_extendedNodes->append(this);
    }

    void Node::prependExtension(Node *node) {
        if(!node) { throw NullPointerException("NULL value passed to Node::prependExtension()"); }
        if(!_extensions) { _extensions = new NodeList; }
        if(_extensions->contains(node)) { throw DuplicateException("Duplicate node passed to Node::prependExtension()"); }
        _extensions->prepend(node);
        if(!node->_extendedNodes) { node->_extendedNodes = new NodeList; }
        node->_extendedNodes->append(this);
    }

    void Node::removeExtension(Node *node) {
        if(!node) { throw NullPointerException("NULL value passed to Node::removeExtension()"); }
        if(!_extensions || !_extensions->contains(node)) { throw NotFoundException("Missing node passed to Node::removeExtension()"); }
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
        return(hash);
    }

    bool Node::hasDirectParent(Node *node) const {
        if(!node) { throw NullPointerException("NULL value passed to Node::hasDirectParent()"); }
        return(node->_children && !node->_children->key(const_cast<Node *>(this)).isEmpty());
    }

    void Node::addParent(const QString &name, Node *node) {
        NamedNode namedNode = NamedNode(name, node);
        if(!_parents) { _parents = new NamedNodeSet; }
        if(_parents->contains(namedNode)) { throw DuplicateException("Duplicate node/name passed to Node::addParent()"); }
        if(!node->_children) { node->_children = new NodeHash; }
        if(node->_children->contains(name)) { throw DuplicateException("Duplicate name in parent passed to Node::addParent()"); }
        _parents->insert(namedNode);
        node->_children->insert(name, this);
    }

    void Node::removeParent(const QString &name, Node *node) {
        NamedNode namedNode = NamedNode(name, node);
        if(!_parents || !_parents->contains(namedNode)) { throw NotFoundException("Missing node/name passed to Node::removeParent()"); }
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
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Node::hasDirectChild()"); }
        return(_children && _children->contains(name));
    }

    Node *Node::directChild(const QString &name) const {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Node::directChild()"); }
        if(!_children) { throw NotFoundException("Missing name passed to Node::directChild()"); }
        Node *value = _children->value(name);
        if(!value) { throw NotFoundException("Missing name passed to Node::directChild()"); }
        return(value);
    }

    Node *Node::addDirectChild(const QString &name, Node *node) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Node::addDirectChild()"); }
        if(!node) { throw NullPointerException("NULL value passed to Node::addDirectChild()"); }
        if(_children && _children->contains(name)) { throw DuplicateException("Duplicate name passed to Node::addDirectChild()"); }
        node->addParent(name, this);
        return(node);
    }

    Node *Node::setDirectChild(const QString &name, Node *node) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Node::setDirectChild()"); }
        if(!node) { throw NullPointerException("NULL value passed to Node::setDirectChild()"); }
        if(!_children || !_children->contains(name)) { throw NotFoundException("Missing name passed to Node::setDirectChild()"); }
        Node *currentChild = directChild(name);
        if(currentChild != node) {
            currentChild->removeParent(name, this);
            node->addParent(name, this);
        }
        return(node);
    }

    Node *Node::addOrSetDirectChild(const QString &name, Node *node) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Node::addOrSetDirectChild()"); }
        if(!node) { throw NullPointerException("NULL value passed to Node::addOrSetDirectChild()"); }
        if(!_children || !_children->contains(name)) {
            node->addParent(name, this);
        } else {
            Node *currentChild = directChild(name);
            if(currentChild != node) {
                currentChild->removeParent(name, this);
                node->addParent(name, this);
            }
        }
        return(node);
    }

    void Node::removeDirectChild(const QString &name) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Node::removeDirectChild()"); }
        if(!_children) { throw NotFoundException("Missing name passed to Node::removeDirectChild()"); }
        Node *node = _children->value(name);
        if(!node) { throw NotFoundException("Missing name passed to Node::removeDirectChild()"); }
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
                if(returnThisIfFound) { return(this); }
                NamedNodeList parentPath;
                while(parent != this) {
                    NamedNode par = parentTree.value(parent);
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
                return(child);
            }
        }
        return(NULL);
    }

    bool Node::hasChild(const QString &name) const {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Node::hasChild()"); }
        return(const_cast<Node *>(this)->_getOrSetChild(name, NULL, true));
    }

    Node *Node::child(const QString &name) const {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Node::child()"); }
        Node *child = const_cast<Node *>(this)->_getOrSetChild(name);
        if(!child) { throw NotFoundException("Child not found in child()"); }
        return(child);
    }

    Node *Node::setChild(const QString &name, Node *node) {
        if(name.isEmpty()) { throw ArgumentException("Empty name passed to Node::setChild()"); }
        if(!node) { throw NullPointerException("NULL value passed to Node::setChild()"); }
        Node *child = _getOrSetChild(name, node);
        if(!child) { throw NotFoundException("Child not found in Node::setChild()"); }
        return(child);
    }

    const QString Node::inspect() const {
        QString str;
        str = uniqueHexID();
        str.append(": [");
        str.append(QStringList(parents().keys()).join(", "));
        str.append("] => [");
        str.append(QStringList(children().keys()).join(", "));
        str.append("]");
        return str;
    }
}
