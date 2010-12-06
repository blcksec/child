#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QHash>
#include <QtCore/QStack>

#include "child.h"
#include "child/toolbox.h"

CHILD_BEGIN

typedef unsigned long long int HugeUnsignedInteger;

// === NodePtr ===

class Node;

#define CHILD_PTR_DECLARATION(NAME, ORIGIN) \
class NAME; \
class NAME##Ptr : public ORIGIN##Ptr { \
public: \
    NAME##Ptr() {} \
    explicit NAME##Ptr(const NAME &other); \
    explicit NAME##Ptr(const Node &other, bool dynamicCast = false); \
    explicit NAME##Ptr(const NAME *other); \
    NAME##Ptr(const NAME##Ptr &other); \
    NAME##Ptr(const NodePtr &other, bool dynamicCast = false); \
    static NAME##Ptr dynamicCast(const NodePtr &other); \
    \
    NAME &operator*(); \
    const NAME &operator*() const; \
    NAME *operator->(); \
    const NAME *operator->() const; \
    NAME##Ptr &operator=(const NAME##Ptr &other); \
};

#define CHILD_PTR_DEFINITION(NAME, ORIGIN) \
inline NAME##Ptr::NAME##Ptr(const NAME &other) { initData(&other); } \
inline NAME##Ptr::NAME##Ptr(const Node &other, bool dynamicCast) { \
    if(dynamicCast) \
        initData(dynamic_cast<const NAME *>(&other)); \
    else { \
        CHILD_NODE_PTR_CHECK_CAST(NAME, &other); \
        initData(&other); \
    } \
} \
inline NAME##Ptr::NAME##Ptr(const NAME *other) { CHILD_NODE_PTR_CHECK_CAST(NAME, other); initData(other); } \
inline NAME##Ptr::NAME##Ptr(const NAME##Ptr &other) : ORIGIN##Ptr(other) {} \
inline NAME##Ptr::NAME##Ptr(const NodePtr &other, bool dynamicCast) { \
    if(dynamicCast) \
        initData(dynamic_cast<const NAME *>(other.data())); \
    else { \
        CHILD_NODE_PTR_CHECK_CAST(NAME, other.data()); \
        initData(other.data()); \
    } \
} \
inline NAME##Ptr NAME##Ptr::dynamicCast(const NodePtr &other) { return NAME##Ptr(dynamic_cast<const NAME *>(other.data())); } \
\
inline NAME &NAME##Ptr::operator*() { CHILD_NODE_PTR_CHECK_DATA; return *static_cast<NAME *>(data()); }; \
inline const NAME &NAME##Ptr::operator*() const { CHILD_NODE_PTR_CHECK_DATA; return *static_cast<const NAME *>(data()); }; \
inline NAME *NAME##Ptr::operator->() { CHILD_NODE_PTR_CHECK_DATA; return static_cast<NAME *>(data()); }; \
inline const NAME *NAME##Ptr::operator->() const { CHILD_NODE_PTR_CHECK_DATA; return static_cast<const NAME *>(data()); }; \
inline NAME##Ptr &NAME##Ptr::operator=(const NAME##Ptr &other) { CHILD_NODE_PTR_CHECK_CAST(NAME, other.data()); setData(other.data()); return *this; }

#define CHILD_NODE_PTR_CHECK_DATA \
if(!data()) CHILD_THROW_NULL_POINTER_EXCEPTION("NULL pointer dereferenced")

#define CHILD_NODE_PTR_CHECK_CAST(NAME, DATA) \
if(DATA && !dynamic_cast<const NAME *>(DATA)) CHILD_THROW_TYPECAST_EXCEPTION("typecasting failed")

class NodePtr {
public:
    NodePtr();
    explicit NodePtr(const Node &other);
    explicit NodePtr(const Node *other);
    NodePtr(const NodePtr &other);
    virtual ~NodePtr();

    Node *data();
    const Node *data() const;
    void initData(const Node *data);
    void setData(const Node *data);

    Node &operator*();
    const Node &operator*() const;
    Node *operator->();
    const Node *operator->() const;
    NodePtr &operator=(const NodePtr &other);

    bool isNull() const { return !data(); }
    bool isNotNull() const { return data(); }
    operator bool() const { return !isNull(); }
    bool operator!() const { return isNull(); }
private:
    const Node *_data;
};

// === NodeRef ===

class NodeRef : public NodePtr { // Can be used as key in hash (comparisons by value)
public:
    NodeRef() : NodePtr() {}
    NodeRef(const NodePtr &other) : NodePtr(other) {}
};

// === Node ===

#define CHILD_NODE(ARGS...) NodePtr(new Node(Node::findInContext("Node"), ##ARGS))

#define CHILD_DECLARATION(NAME, ORIGIN) \
public: \
    static NAME##Ptr &root(); \
    virtual const QString className() const { return #NAME; } \
    static const bool isInitialized; \
private:

#define CHILD_DEFINITION(NAME, ORIGIN) \
const bool NAME::isInitialized = NAME::root().isNotNull(); \
NAME##Ptr &NAME::root() { \
    static NAME##Ptr _root; \
    if(!_root) { \
        _root = NAME##Ptr(new NAME(ORIGIN::root())); \
        initRoot(); \
    } \
    return _root; \
}

#define CHILD_THROW_RUNTIME_EXCEPTION(MESSAGE) \
Node::throwRuntimeException(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)

#define CHILD_THROW_NULL_POINTER_EXCEPTION(MESSAGE) \
Node::throwNullPointerException(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)

#define CHILD_THROW_TYPECAST_EXCEPTION(MESSAGE) \
Node::throwTypecastException(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)

#define CHILD_CHECK_NODE_PTR(NODE_PTR) \
if(!(NODE_PTR)) CHILD_THROW_NULL_POINTER_EXCEPTION("NodePtr is NULL")

typedef QList<NodePtr> NodeList;
typedef QHash<NodeRef, NodePtr> NodeHash;
typedef QHashIterator<NodeRef, NodePtr> NodeHashIterator;

class Node {
    CHILD_DECLARATION(Node, NULL);
public:
    friend class NodePtr;

    enum Comparison { Smaller = -2, SmallerOrEqual, Equal, GreaterOrEqual, Greater, Different };

    Node(const NodePtr &origin) : _origin(origin), _extensions(NULL), // default constructor
        _children(NULL), _parents(NULL), _refCount(0) { nodeCount()++; }

    Node(const Node &other) : _origin(other._origin), _extensions(NULL), // copy constructor
        _children(NULL), _parents(NULL), _refCount(0) {
        if(other._extensions) _extensions = new NodeList(*other._extensions);
        if(other._children) {
            QHashIterator<QString, NodePtr> i(other.children());
            while(i.hasNext()) { i.next(); addChild(i.key(), i.value()); }
        }
        nodeCount()++;
    }

    virtual ~Node();

    static void initRoot() { root()->addChild("Node", root()); }
    virtual NodePtr fork() const { return NodePtr(new Node(NodePtr(this))); }

    const NodePtr &origin() const { return _origin; }
    void setOrigin(const NodePtr &node);

    NodeList extensions() const { return _extensions ? *_extensions : NodeList(); }

    void addExtension(const NodePtr &node);
    void prependExtension(const NodePtr &node);
    void removeExtension(const NodePtr &node);
    void removeAllExtensions();
    bool hasExtension(const NodePtr &node) const;

    const NodePtr child(const QString &name) const;
    const NodePtr addChild(const QString &name, const NodePtr &value);
    const NodePtr setChild(const QString &name, const NodePtr &value);
    void removeChild(const QString &name);

    void addAnonymousChild(const NodePtr &value) {
        CHILD_CHECK_NODE_PTR(value);
        value->_addParent(this);
    }

    void removeAnonymousChild(const NodePtr &value) {
        CHILD_CHECK_NODE_PTR(value);
        value->_removeParent(this);
    }

    const NodePtr hasChild(const QString &name, bool searchInParents = true,
                           bool forkChildFoundInFirstOrigin = true, bool *isDirectPtr = NULL) const;

    const NodePtr hasDirectChild(const QString &name, bool *isRemovedPtr = NULL) const {
        NodePtr child;
        if(_children) child = _children->value(name);
        if(isRemovedPtr) *isRemovedPtr = !child && _children && _children->contains(name);
        return child;
    }

    const QString hasDirectChild(const NodePtr &value) const {
        return _children ? _children->key(value) : QString();
    }

    bool hasDirectParent(const NodePtr &parent) const {
        CHILD_CHECK_NODE_PTR(parent);
        return _parents && _parents->contains(parent.data());
    }

    const QHash<QString, NodePtr> children() const;
    const NodeList parents() const;

    virtual Comparison compare(const Node &other) const { return this == &other ? Equal : Different; }
    virtual uint hash() const { return ::qHash(this); }

    static HugeUnsignedInteger &nodeCount() {
        static HugeUnsignedInteger _nodeCount = 0;
        return _nodeCount;
    }

    static const NodePtr context() {
        if(contextStack().isEmpty()) qFatal("Fatal error: context stack is empty!");
        return contextStack().top();
    }

    static void pushContext(const NodePtr &node) { contextStack().push(node); }

    static const NodePtr popContext() {
        if(contextStack().isEmpty()) qFatal("Fatal error: context stack is empty!");
        return contextStack().pop();
    }

    static QStack<NodePtr> &contextStack() {
        static QStack<NodePtr> _contextStack;
        return _contextStack;
    }

    static NodePtr findInContext(const QString &name) { return context()->child(name); }

    static void throwRuntimeException(const QString &message = "", const QString &file = "",
                                   const int line = 0, const QString &function = "");

    static void throwNullPointerException(const QString &message = "", const QString &file = "",
                                   const int line = 0, const QString &function = "");

    static void throwTypecastException(const QString &message = "", const QString &file = "",
                                   const int line = 0, const QString &function = "");

    const long long int memoryAddress() const { return reinterpret_cast<long long int>(this); }
    const QString hexMemoryAddress() const { return QString("0x%1").arg(memoryAddress(), 0, 16); }
    virtual const QString inspect() const;
private:
    NodePtr _origin;
    NodeList *_extensions;
    QHash<QString, NodePtr> *_children;
    mutable QHash<const Node *, HugeUnsignedInteger> *_parents;
    mutable HugeUnsignedInteger _refCount;

    void _setChild(const QString &name, const NodePtr &value);
    void _addParent(const Node *parent) const;
    void _removeParent(const Node *parent) const;

    void retain() const { _refCount++; }
    void release() const { if(--_refCount == 0) delete this; }
};

inline bool operator==(const Node &a, const Node &b) { return a.compare(b) == Node::Equal; }
inline bool operator!=(const Node &a, const Node &b) { return a.compare(b) != Node::Equal; }
inline uint qHash(const Node &node) { return node.hash(); }

// === NodePtr inline definitions ===

inline NodePtr::NodePtr() : _data(NULL) {}
inline NodePtr::NodePtr(const Node &other) { initData(&other); }
inline NodePtr::NodePtr(const Node *other) { initData(other); }
inline NodePtr::NodePtr(const NodePtr &other) { initData(other.data()); }
inline NodePtr::~NodePtr() { if(data()) data()->release(); }

inline Node *NodePtr::data() { return const_cast<Node *>(_data); }
inline const Node *NodePtr::data() const { return _data; }

inline void NodePtr::initData(const Node *data) {
    if(data) data->retain();
    _data = data;
}

inline void NodePtr::setData(const Node *data) {
    if(_data != data) {
        if(_data) _data->release();
        if(data) data->retain();
        _data = data;
    }
}

inline Node &NodePtr::operator*() { CHILD_NODE_PTR_CHECK_DATA; return *data(); };
inline const Node &NodePtr::operator*() const { CHILD_NODE_PTR_CHECK_DATA; return *data(); };
inline Node *NodePtr::operator->() { CHILD_NODE_PTR_CHECK_DATA; return data(); };
inline const Node *NodePtr::operator->() const { CHILD_NODE_PTR_CHECK_DATA; return data(); };
inline NodePtr &NodePtr::operator=(const NodePtr &other) { setData(other.data()); return *this; }

inline bool operator==(const NodePtr &a, const NodePtr &b) { return a.data() == b.data(); }
inline bool operator!=(const NodePtr &a, const NodePtr &b) { return a.data() != b.data(); }
inline uint qHash(const NodePtr &node) { return ::qHash(node.data()); }

// === NodeRef inline definitions ===

inline bool operator==(const NodeRef &a, const NodeRef &b) { return a->compare(*b) == Node::Equal; }
inline bool operator!=(const NodeRef &a, const NodeRef &b) { return a->compare(*b) != Node::Equal; }
inline uint qHash(const NodeRef &node) { return node->hash(); }

CHILD_END

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
