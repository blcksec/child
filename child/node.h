#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QQueue>
#include <QtCore/QStack>

#include "child/toolbox.h"

#define CHILD_DECLARATION(NAME, ORIGIN, PARENT) \
public: \
    static NAME##Ptr &root(); \
    static void initRoot(); \
    virtual NAME *_new() const { return new NAME; } \
    virtual const QString className() const { return #NAME; } \
private: \
    static NAME##Ptr _root;

#define CHILD_DEFINITION(NAME, ORIGIN, PARENT) \
NAME##Ptr NAME::_root = NAME::root(); \
NAME##Ptr &NAME::root() { \
    if(!_root) { \
        _root = NAME##Ptr(new NAME); \
        _root->setOrigin(ORIGIN::root()); \
        PARENT::root()->setChild(#NAME, _root); \
        NAME::initRoot(); \
    } \
    return _root; \
}

#define CHILD_PTR_DECLARATION(NAME, ORIGIN, PARENT) \
class NAME; \
class NAME##Ptr : public ORIGIN##Ptr { \
public: \
    NAME##Ptr() {} \
    explicit NAME##Ptr(const NAME *data); \
    NAME##Ptr(const NodePtr &other); \
    NAME &operator*(); \
    const NAME &operator*() const; \
    NAME *operator->(); \
    const NAME *operator->() const; \
    const NAME *setData(const NAME *data); \
    NAME##Ptr& operator=(const NAME##Ptr &other); \
};

#define CHILD_PTR_DEFINITION(NAME, ORIGIN, PARENT) \
inline NAME##Ptr::NAME##Ptr(const NAME *data) : ORIGIN##Ptr(data) {} \
inline NAME##Ptr::NAME##Ptr(const NodePtr &other) : ORIGIN##Ptr(other) {} \
inline NAME &NAME##Ptr::operator*() { return *static_cast<NAME *>(NodePtr::data()); }; \
inline const NAME &NAME##Ptr::operator*() const { return *static_cast<const NAME *>(NodePtr::data()); }; \
inline NAME *NAME##Ptr::operator->() { return static_cast<NAME *>(NodePtr::data()); }; \
inline const NAME *NAME##Ptr::operator->() const { return static_cast<const NAME *>(NodePtr::data()); }; \
inline const NAME *NAME##Ptr::setData(const NAME *data) { return static_cast<const NAME *>(NodePtr::setData(data)); } \
inline NAME##Ptr& NAME##Ptr::operator=(const NAME##Ptr &other) { NodePtr::setData(other._data); return *this; }

namespace Child {

class Node;

class NodePtr {
public:
    NodePtr();
    explicit NodePtr(const Node *data);
    NodePtr(const NodePtr &other);
    virtual ~NodePtr();

    Node &operator*();
    const Node &operator*() const;
    Node *operator->();
    const Node *operator->() const;
    const Node *setData(const Node *data);
    NodePtr& operator=(const NodePtr &other);

    bool isNull() const { return !_data; }
    operator bool() const { return !isNull(); }
    bool operator!() const { return isNull(); }
    bool operator==(const NodePtr &other) const { return _data == other._data; }
    bool operator==(const Node *other) const { return _data == other; }
    bool operator!=(const NodePtr &other) const { return _data != other._data; }
    bool operator!=(const Node *other) const { return _data != other; }
protected:
    const Node *_data;

    Node *data();
    const Node *data() const;
};

class Node {
public:
    friend class NodePtr;

    Node() : _extensions(NULL), _children(NULL), _parents(NULL), _refCount(0) {
        _nodeCount++;
    }

    virtual ~Node();

    static NodePtr &root();
    static void initRoot();

    static NodePtr make();

    NodePtr fork() const {
        NodePtr f(_new());
        f->setOrigin(NodePtr(this));
        f->initFork();
        return f;
    }

    virtual Node *_new() const { return new Node; }

    void initFork() {}

    virtual const QString className() const { return "Node"; }

    const NodePtr &origin() const { return _origin; }
    void setOrigin(const NodePtr &node) { checkNodePtr(node); _origin = node != this ? node : NodePtr(); }

    QList<NodePtr> extensions() const { return _extensions ? *_extensions : QList<NodePtr>(); }
    bool hasExtension(const NodePtr &node) const { checkNodePtr(node); return _extensions && _extensions->contains(node); }
    void addExtension(const NodePtr &node);
    void prependExtension(const NodePtr &node);
    void removeExtension(const NodePtr &node);
    void removeAllExtensions();

    const NodePtr hasDirectChild(const QString &name) const {
        return _children ? _children->value(name) : NodePtr();
    }

    const QString hasDirectChild(const NodePtr &value) const {

        return _children ? _children->key(value) : QString();
    }

    const NodePtr child(const QString &name) const {
        NodePtr node = hasDirectChild(name);
        if(!node)
            if(origin() && (node = origin()->child(name))) {
                node = node->fork();
                const_cast<Node *>(this)->_setChild(name, node);
            } else
                qFatal("child not found");
        return node;
    }

    const NodePtr setChild(const QString &name, const NodePtr &value) {
        if(NodePtr current = hasDirectChild(name)) {
            if(current == value) return value;
            current->_removeParent(this);
        }
        _setChild(name, value);
        return value;
    }

    void _setChild(const QString &name, const NodePtr &value) {
        if(!_children) _children = new QHash<QString, NodePtr>;
        _children->insert(name, value);
        value->_addParent(this);
    }

    void _addParent(Node *parent) const {
        HugeUnsignedInteger count = 0;
        if(_parents)
            count = _parents->value(parent);
        else
            _parents = new QHash<Node *, HugeUnsignedInteger>;
        _parents->insert(parent, count + 1);
    }

    void _removeParent(Node *parent) const {
        if(!_parents) qFatal("parent not found");
        HugeUnsignedInteger count = _parents->value(parent) - 1;
        if(count > 0)
            _parents->insert(parent, count);
        else if(count == 0)
            _parents->remove(parent);
        else
            qFatal("parent not found");
    }

    const QHash<QString, NodePtr> children() const { return _children ? *_children : QHash<QString, NodePtr>(); }


    bool hasDirectParent(const NodePtr &parent) const {
        checkNodePtr(parent);
        return !parent->hasDirectChild(NodePtr(this)).isNull();
    }

    static const HugeUnsignedInteger nodeCount() {
        return _nodeCount;
    }

    const long long int memoryAddress() const { return reinterpret_cast<long long int>(this); }
    const QString hexMemoryAddress() const { return QString("0x%1").arg(memoryAddress(), 0, 16); }
    virtual const QString inspect() const;
private:
    static NodePtr _root;
    NodePtr _origin;
    QList<NodePtr> *_extensions;
    QHash<QString, NodePtr> *_children;
    mutable QHash<Node *, HugeUnsignedInteger> *_parents;
    mutable HugeUnsignedInteger _refCount;
    static HugeUnsignedInteger _nodeCount;

    void retain() const { _refCount++; }
    void release() const { if(--_refCount == 0) delete this; }

    void checkNodePtr(const NodePtr &node) const {
        if(!node) qFatal("Node pointer is NULL");
    }
};

inline Node &NodePtr::operator*() { return *data(); };
inline const Node &NodePtr::operator*() const { return *data(); };
inline Node *NodePtr::operator->() { return data(); };
inline const Node *NodePtr::operator->() const { return data(); };
inline NodePtr& NodePtr::operator=(const NodePtr &other) { setData(other._data); return *this; }
inline Node *NodePtr::data() {
    if(!_data) qFatal("NULL pointer dereferenced");
    return const_cast<Node *>(_data);
}
inline const Node *NodePtr::data() const {
    if(!_data) qFatal("NULL pointer dereferenced");
    return _data;
}

} // namespace Child

/*
#define CHILD_DECLARATION(NAME, ORIGIN, PARENT) \
public: \
    inline static const QString className() { return #NAME; } \
    static NAME *root(); \
    static void initRoot(); \
    inline static NAME *fork(Node *world) { \
        NAME::root(); \
        return static_cast<NAME *>(world->child(#PARENT)->child(#NAME))->fork(); \
    } \
    inline virtual NAME *fork() { \
        NAME *f = new NAME; \
        f->setOrigin(this); \
        f->initFork(); \
        return f; \
    } \
    inline static NAME *as(Node *node) { return static_cast<NAME *>(node); } \
    inline static const NAME *as(const Node *node) { return static_cast<const NAME *>(node); } \
    inline static NAME *is(Node *node) { return dynamic_cast<NAME *>(node); } \
    inline static const NAME *is(const Node *node) { return dynamic_cast<const NAME *>(node); } \
private:

#define CHILD_DEFINITION(NAME, ORIGIN, PARENT) \
NAME *NAME::root() { \
    static NAME *_root; \
    if(!_root) { \
        _root = new NAME; \
        _root->setOrigin(ORIGIN::root()); \
        _root->addParent(#NAME, PARENT::root()); \
        NAME::initRoot(); \
    } \
    return _root; \
}

namespace Child {
    class Node;
    class NumberedNode;
    class NamedNode;
    class NodeRef;

    typedef QList<Node *> NodeList;
    typedef QList<Node *> NodeLinkedList;
    typedef QHash<QString, Node *> NodeHash;
    typedef QHashIterator<QString, Node *> NodeHashIterator;
    typedef QMultiHash<QString, Node *> NodeMultiHash;
    typedef QSet<Node *> NodeSet;
    typedef QQueue<Node *> NodeQueue;

    typedef QList<NumberedNode> NumberedNodeList;
    typedef QSet<NumberedNode> NumberedNodeSet;

    typedef QList<NamedNode> NamedNodeList;
    typedef QSet<NamedNode> NamedNodeSet;

    typedef Node *(Node::*NodeMethodPtr)();

    typedef uint NumberId;
    static const NumberId nullNumberId = 0;
    static const NumberId firstNumberId = 1;
    typedef QList<NumberId> NumberIdList;
    typedef QHash<NodeRef, NumberId> NumberIdHash;
    typedef QSet<NumberId> NumberIdSet;

    class Node {
        CHILD_DECLARATION(Node, Node, Node);
    public:
        enum Comparison { Smaller = -2, SmallerOrEqual, Equal, GreaterOrEqual, Greater, Different };

        Node() :
            _origin(NULL), _forks(NULL), _extensions(NULL), _extendedNodes(NULL),
            _parents(NULL), _children(NULL), _isVirtual(false) {
            nodeCount()++;
        }

        virtual ~Node();

        virtual void initFork() {}

        static long long int &nodeCount() {
            static long long int _nodeCount = 0;
            return _nodeCount;
        }

        bool isVirtual() const { return _isVirtual; }
        Node *setIsVirtual(bool value) { _isVirtual = value; return this; }

        Node *origin() const { return _origin; }
        Node *setOrigin(Node *node);
        void unsetOrigin() { if(_origin) { _origin->_forks->removeOne(this); _origin = NULL; } }
        NodeList forks() const { return _forks ? NodeList(*_forks) : NodeList(); }
        bool directOriginIs(Node *node) const;
        void removeAllForks();

        NodeList extensions() const { return _extensions ? NodeList(*_extensions) : NodeList(); }
        NodeList extendedNodes() const { return _extendedNodes ? NodeList(*_extendedNodes) : NodeList(); }
        bool hasExtension(Node *node) const;
        void addExtension(Node *node);
        void prependExtension(Node *node);
        void removeExtension(Node *node);
        void removeAllExtensions();
        void removeAllExtendedNodes();

        const NodeMultiHash parents() const;
        const NodeHash children() const { return _children ? NodeHash(*_children) : NodeHash(); }
        bool hasDirectParent(Node *node) const;
        void addParent(const QString &name, Node *node);
        void removeParent(const QString &name, Node *node);
        void removeAllParents();
        bool hasDirectChild(const QString &name) const;
        Node *directChild(const QString &name) const;
        Node *addDirectChild(const QString &name, Node *node);
        Node *setDirectChild(const QString &name, Node *node);
        Node *addOrSetDirectChild(const QString &name, Node *node);
        void removeDirectChild(const QString &name);
        void removeAllDirectChildren();
        Node *_getOrSetChild(const QString &name, Node *setValue = NULL, bool returnThisIfFound = false);
        bool hasChild(const QString &name) const;
        Node *child(const QString &name) const;
        Node *setChild(const QString &name, Node *node);
        void deleteIfOrphan() { if(!_parents || _parents->empty()) { delete this; } }

        virtual Comparison compare(const Node *other) const { return this == other ? Equal : Different; }
        virtual uint hash() const { return ::qHash(this); }

        const QString numberIdToName(const NumberId id) const { return QString("\\%1").arg(id); }

        const long long int memoryAddress() const { return reinterpret_cast<long long int>(this); }
        const QString hexMemoryAddress() const { return QString("0x%1").arg(memoryAddress(), 0, 16); }
        virtual const QString inspect() const;
    private:
        Node *_origin;
        NodeList *_forks; // backlink cache
        NodeList *_extensions;
        NodeList *_extendedNodes; // backlink cache
        NamedNodeSet *_parents;
        NodeHash *_children; // backlink cache
        bool _isVirtual : 1;

        Node(const Node &node); // disabled copy constructor

        void checkName(const QString &name) const {
            if(name.isEmpty()) throw ArgumentException("child name is empty");
        }

        void checkNode(Node *node) const {
            if(!node) throw NullPointerException("Node pointer is NULL");
        }
    };

    inline bool operator==(const Node &a, const Node &b) { return a.compare(&b) == Node::Equal; }
    inline bool operator!=(const Node &a, const Node &b) { return a.compare(&b) != Node::Equal; }
    inline uint qHash(const Node &node) { return node.hash(); }

    class NumberedNode {
    public:
        NumberedNode(const int number, Node *const node) : number(number), node(node) {
            if(!node) throw NullPointerException("NULL node passed to NumberedNode constructor");
        }
        int number;
        Node *node;
    };

    inline bool operator==(const NumberedNode &a, const NumberedNode &b) {
        return a.node == b.node && a.number == b.number;
    }

    inline uint qHash(const NumberedNode &key) {
        return ::qHash(key.number) ^ ::qHash(key.node);
    }

    class NamedNode {
    public:
        NamedNode(const QString &name = "", Node *const node = NULL) : name(name), node(node) {
            if(name.isEmpty()) throw ArgumentException("empty child name passed to NamedNode constructor");
            if(!node) throw NullPointerException("NULL node passed to NamedNode constructor");
        }
        QString name;
        Node *node;
    };

    inline bool operator==(const NamedNode &a, const NamedNode &b) {
        return a.node == b.node && a.name == b.name;
    }

    inline uint qHash(const NamedNode &key) {
        return qHash(key.name) ^ ::qHash(key.node);
    }

    class NodeRef {
    public:
        Node *node;

        NodeRef(Node *const node) : node(node) {
            if(!node) throw NullPointerException("NULL node passed to NodeRef constructor");
        }

        Node &operator*() const { return *node; }
    };

    inline bool operator==(const NodeRef &a, const NodeRef &b) {
        return &a == &b || a.node->compare(b.node) == Node::Equal;
    }

    inline bool operator!=(const NodeRef &a, const NodeRef &b) {
        return &a != &b && a.node->compare(b.node) != Node::Equal;
    }

    inline uint qHash(const NodeRef &ref) { return ref.node->hash(); }
}
*/

#endif // CHILD_NODE_H
