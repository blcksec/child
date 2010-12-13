#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QHash>
#include <QtCore/QStack>

#include "child/global.h"
#include "child/pointer.h"

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
    NAME##Ptr(const NAME *other); \
    NAME##Ptr(const NAME##Ptr &other); \
    NAME##Ptr(const NodePtr &other, bool dynamicCast = false); \
    static NAME##Ptr dynamicCast(const NodePtr &other); \
    \
    NAME &operator*(); \
    const NAME &operator*() const; \
    NAME *operator->(); \
    const NAME *operator->() const; \
    NAME##Ptr &operator=(const NAME##Ptr &other); \
    static const NAME##Ptr &null() { static const NAME##Ptr _null; return _null; }; \
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
inline NAME##Ptr NAME##Ptr::dynamicCast(const NodePtr &other) { return dynamic_cast<const NAME *>(other.data()); } \
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
    NodePtr(const Node *other);
    NodePtr(const NodePtr &other);
    virtual ~NodePtr();

    Node *data();
    const Node *data() const;
    void initData(const Node *data);
    void setData(const Node *data);
    void clear() { setData(NULL); }

    Node &operator*();
    const Node &operator*() const;
    Node *operator->();
    const Node *operator->() const;
    NodePtr &operator=(const NodePtr &other);

    bool isNull() const { return !data(); }
    bool isNotNull() const { return data(); }
    operator bool() const { return !isNull(); }
    bool operator!() const { return isNull(); }

    static const NodePtr &null() { static const NodePtr _null; return _null; };
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

#define CHILD_NODE(ARGS...) NodePtr(new Node(Node::context()->child("Node"), ##ARGS))

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

#define CHILD_CHECK_NODE_PTR(NODE_PTR) \
if(!(NODE_PTR)) CHILD_THROW_NULL_POINTER_EXCEPTION("NodePtr is NULL")

typedef QList<NodePtr> NodeList;
typedef QHash<NodeRef, NodePtr> NodeHash;
typedef QHashIterator<NodeRef, NodePtr> NodeHashIterator;

class Node {
    CHILD_DECLARATION(Node, NULL);
public:
    friend class NodePtr;
    friend class GenericPointer<Node>;

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
    virtual NodePtr fork() const { return new Node(this); }

    const NodePtr &origin() const { return _origin; }
    void setOrigin(const NodePtr &node);

    NodeList extensions() const { return _extensions ? *_extensions : NodeList(); }

    void addExtension(const NodePtr &node);
    void prependExtension(const NodePtr &node);
    void removeExtension(const NodePtr &node);
    void removeAllExtensions();
    bool hasExtension(const NodePtr &node) const;

    const NodePtr child(const QString &name) const;

    const NodePtr child(const QString &name1, const QString &name2) const {
        return child(name1)->child(name2);
    }

    const NodePtr child(const QString &name1, const QString &name2, const QString &name3) const {
        return child(name1)->child(name2)->child(name3);
    }

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
        if(contextStack().isEmpty())
            qFatal("Fatal error: context stack is empty!");
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

    const long long int memoryAddress() const { return reinterpret_cast<long long int>(this); }
    const QString hexMemoryAddress() const { return QString("0x%1").arg(memoryAddress(), 0, 16); }
    virtual const QString toString(bool debug = false) const;
    void inspect() const { P(toString(true).toUtf8()); }
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

#endif // CHILD_NODE_H
