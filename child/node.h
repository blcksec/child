#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QQueue>
#include <QtCore/QStack>

#include "child/toolbox.h"

namespace Child {

class Node;

class NodePtr {
public:
    NodePtr();
    NodePtr(Node *data);
    NodePtr(const NodePtr &other);
    virtual ~NodePtr();

    virtual Node *initData();
    Node *data() const;
    Node &operator*();
    const Node &operator*() const;
    Node *operator->();
    const Node *operator->() const;
    Node *setData(Node *data);
    NodePtr& operator=(const NodePtr &other);

    bool isNull() const { return !_data; }
    operator bool() const { return !isNull(); }
    bool operator!() const { return isNull(); }
    bool operator==(const NodePtr &other) const { return _data == other._data; }
    bool operator!=(const NodePtr &other) const { return _data != other._data; }
private:
    Node *_data;
};

class Node {
public:
    friend class NodePtr;

    Node(const NodePtr &origin = NodePtr()) : _origin(origin), _extensions(NULL),
        _children(NULL), _parents(NULL), _refCount(0) { nodeCount()++; }

    virtual ~Node();

    static unsigned long long int &nodeCount() {
        static unsigned long long int _nodeCount = 0;
        return _nodeCount;
    }

    const NodePtr fork() const {
        NodePtr node;
        node->_origin = const_cast<Node *>(this);
        return node;
    }

    const NodePtr &origin() const { return _origin; }
    void setOrigin(const NodePtr &node) { checkNodePtr(node); _origin = node; }

    QList<NodePtr> extensions() const { return _extensions ? *_extensions : QList<NodePtr>(); }
    bool hasExtension(const NodePtr &node) const { checkNodePtr(node); return _extensions && _extensions->contains(node); }
    void addExtension(const NodePtr &node);
    void prependExtension(const NodePtr &node);
    void removeExtension(const NodePtr &node);
    void removeAllExtensions();

    const NodePtr hasDirectChild(const QString &name) const {
        return _children ? _children->value(name) : NodePtr();
    }

    const NodePtr child(const QString &name) const {
        if(NodePtr node = hasDirectChild(name)) return node;
        if(_origin) {
            NodePtr node = _origin->child(name);
            if(node) {
                node = node->fork();
                const_cast<Node *>(this)->_setChild(name, node);
                return node;
            }
        }
        qFatal("child not found");
    }

    const NodePtr setChild(const QString &name, const NodePtr &value) {
        if(NodePtr current = hasDirectChild(name)) {
            if(current == value) return(value);
            current->_removeParent(this);
        }
        _setChild(name, value);
        return(value);
    }

    void _setChild(const QString &name, const NodePtr &value) {
        if(!_children) _children = new QHash<QString, NodePtr>;
        _children->insert(name, value);
        value->_addParent(this);
    }

    void _addParent(Node *parent) const {
        unsigned long long int count = 0;
        if(_parents)
            count = _parents->value(parent);
        else
            _parents = new QHash<Node *, unsigned long long int>;
        _parents->insert(parent, count + 1);
    }

    void _removeParent(Node *parent) const {
        if(!_parents) qFatal("parent not found");
        unsigned long long int count = _parents->value(parent) - 1;
        if(count > 0)
            _parents->insert(parent, count);
        else if(count == 0)
            _parents->remove(parent);
        else
            qFatal("parent not found");
    }

private:
    NodePtr _origin;
    QList<NodePtr> *_extensions;
    QHash<QString, NodePtr> *_children;
    mutable QHash<Node *, unsigned long long int> *_parents;
    unsigned long long int _refCount;

    void retain() { _refCount++; }
    void release() { if(--_refCount == 0) delete this; }

    void checkNodePtr(const NodePtr &node) const {
        if(!node) throw NullPointerException("Node pointer is NULL");
    }
};

inline Node *NodePtr::data() const { return _data ? _data : const_cast<NodePtr *>(this)->initData(); }
inline Node &NodePtr::operator*() { return *data(); };
inline const Node &NodePtr::operator*() const { return *data(); };
inline Node *NodePtr::operator->() { return data(); };
inline const Node *NodePtr::operator->() const { return data(); };
inline NodePtr& NodePtr::operator=(const NodePtr &other) { setData(other._data); return *this; }

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
