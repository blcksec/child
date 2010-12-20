#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QHash>
#include <QtCore/QStack>

#include "child/global.h"
#include "child/pointer.h"

CHILD_BEGIN

class MessagePointer;

#define CHILD_NATIVE_METHOD_DECLARE(METHOD) \
Pointer _##METHOD##_(const MessagePointer &message)

#define CHILD_NATIVE_METHOD_DEFINE(NAME, METHOD) \
Pointer NAME::_##METHOD##_(const MessagePointer &message)

#define CHILD_NODE(ARGS...) Pointer(new Node(Node::context()->child("Node"), ##ARGS))

#define CHILD_CHECK_POINTER(POINTER) \
if(!(POINTER)) CHILD_THROW_NULL_POINTER_EXCEPTION("Pointer is NULL")

typedef QList<Pointer> PointerList;
typedef QHash<Reference, Pointer> ReferenceHash;

class Node {
public:
    friend class GenericPointer<Node>;

    static const bool isInitialized;

    explicit Node(const Pointer &origin) : _origin(origin), _extensions(NULL), // default constructor
        _children(NULL), _parents(NULL), _refCount(0) { nodeCount()++; }

    Node(const Node &other) : _origin(other._origin), _extensions(NULL), // copy constructor
        _children(NULL), _parents(NULL), _refCount(0) {
        if(other._extensions) _extensions = new PointerList(*other._extensions);
        if(other._children) {
            QHashIterator<QString, Pointer> i(other.children());
            while(i.hasNext()) { i.next(); addChild(i.key(), i.value()); }
        }
        nodeCount()++;
    }

    virtual ~Node();

    static Pointer &root();
    virtual const QString className() const { return "Node"; }
    static void initRoot();
    virtual Pointer fork() const { return new Node(this); }
    static Pointer forkIfNotNull(const Pointer &node) { return node ? node->fork() : node; }

    const Pointer &origin() const { return _origin; }
    void setOrigin(const Pointer &node);

    PointerList extensions() const { return _extensions ? *_extensions : PointerList(); }

    void addExtension(const Pointer &node);
    void prependExtension(const Pointer &node);
    void removeExtension(const Pointer &node);
    void removeAllExtensions();
    bool hasExtension(const Pointer &node) const;

    Pointer child(const QString &name) const;

    Pointer child(const QString &name1, const QString &name2) const {
        return child(name1)->child(name2);
    }

    Pointer child(const QString &name1, const QString &name2, const QString &name3) const {
        return child(name1)->child(name2)->child(name3);
    }

    Pointer addChild(const QString &name, const Pointer &value);
    Pointer setChild(const QString &name, const Pointer &value);
    void removeChild(const QString &name);

    void addAnonymousChild(const Pointer &value) {
        CHILD_CHECK_POINTER(value);
        value->_addParent(this);
    }

    void removeAnonymousChild(const Pointer &value) {
        CHILD_CHECK_POINTER(value);
        value->_removeParent(this);
    }

    Pointer hasChild(const QString &name, bool searchInParents = true,
                           bool forkChildFoundInFirstOrigin = true, bool *isDirectPointer = NULL) const;

    Pointer hasDirectChild(const QString &name, bool *isRemovedPointer = NULL) const {
        Pointer child;
        if(_children) child = _children->value(name);
        if(isRemovedPointer) *isRemovedPointer = !child && _children && _children->contains(name);
        return child;
    }

    QString hasDirectChild(const Pointer &value) const {
        return _children ? _children->key(value) : QString();
    }

    bool hasDirectParent(const Pointer &parent) const {
        CHILD_CHECK_POINTER(parent);
        return _parents && _parents->contains(parent.data());
    }

    QHash<QString, Pointer> children() const;
    PointerList parents() const;

    virtual Pointer run(const Pointer &receiver = context()) {
        Q_UNUSED(receiver);
        return this;
    }

    virtual Pointer run(const Pointer &receiver, const MessagePointer &message);

    CHILD_NATIVE_METHOD_DECLARE(fork);
private:
    Pointer defineOrAssign(const MessagePointer &message, bool isDefine);
public:
    CHILD_NATIVE_METHOD_DECLARE(define) { return(defineOrAssign(message, true)); }
    CHILD_NATIVE_METHOD_DECLARE(assign) { return(defineOrAssign(message, false)); }

    virtual void hasBeenAssigned(const MessagePointer &message) const {
        Q_UNUSED(message);
    }

    CHILD_NATIVE_METHOD_DECLARE(or);
    CHILD_NATIVE_METHOD_DECLARE(and);
    CHILD_NATIVE_METHOD_DECLARE(not);

    virtual bool isEqualTo(const Pointer &other) const { return this == other.data(); }
    CHILD_NATIVE_METHOD_DECLARE(equal_to);
    CHILD_NATIVE_METHOD_DECLARE(different_from);

    CHILD_NATIVE_METHOD_DECLARE(assert);

    Pointer print() const { P(toString().toUtf8()); return this; }
    CHILD_NATIVE_METHOD_DECLARE(print);

    Pointer inspect() const { P(toString(true).toUtf8()); return this; }
    CHILD_NATIVE_METHOD_DECLARE(inspect);

    long long int memoryAddress() const { return reinterpret_cast<long long int>(this); }
    QString hexMemoryAddress() const { return QString("0x%1").arg(memoryAddress(), 0, 16); }

    virtual bool toBool() const { return true; };

    virtual double toDouble() const {
        CHILD_THROW_CONVERSION_EXCEPTION(QString("cannot convert from %1 to Number").arg(className()));
        return 0;
    };

    virtual QChar toChar() const {
        CHILD_THROW_CONVERSION_EXCEPTION(QString("cannot convert from %1 to Character").arg(className()));
        return 0;
    };

    virtual QString toString(bool debug = false, short level = 0) const;

    virtual uint hash() const { return ::qHash(this); }

    static const Pointer &empty() { static const Pointer _empty(new Node(Node::root())); return _empty; };

    static HugeUnsignedInteger &nodeCount() {
        static HugeUnsignedInteger _nodeCount = 0;
        return _nodeCount;
    }

    static Pointer context() {
        if(contextStack().isEmpty())
            qFatal("Fatal error: context stack is empty!");
        return contextStack().top();
    }

    static void pushContext(const Pointer &node) { contextStack().push(node); }

    static Pointer popContext() {
        if(contextStack().isEmpty()) qFatal("Fatal error: context stack is empty!");
        return contextStack().pop();
    }

    static QStack<Pointer> &contextStack() {
        static QStack<Pointer> _contextStack;
        return _contextStack;
    }

    class ContextPusher {
    public:
        explicit ContextPusher(const Pointer &node) { pushContext(node); }
        ~ContextPusher() { popContext(); }
    private:
        ContextPusher(const ContextPusher &); // prevent copying
        ContextPusher &operator=(const ContextPusher &);
    };
private:
    Pointer _origin;
    PointerList *_extensions;
    QHash<QString, Pointer> *_children;
    mutable QHash<const Node *, HugeUnsignedInteger> *_parents;
    mutable HugeUnsignedInteger _refCount;

    void _setChild(const QString &name, const Pointer &value);
    void _addParent(const Node *parent) const;
    void _removeParent(const Node *parent) const;

    void retain() const { _refCount++; }
    void release() const { if(--_refCount == 0) delete this; }
};

inline bool operator==(const Node &a, const Node &b) { return a.isEqualTo(Pointer(b)); }
inline bool operator!=(const Node &a, const Node &b) { return !a.isEqualTo(Pointer(b)); }
inline uint qHash(const Node &node) { return node.hash(); }

inline bool operator==(const Pointer &a, const Pointer &b) { return a.data() == b.data(); }
inline bool operator!=(const Pointer &a, const Pointer &b) { return a.data() != b.data(); }
inline uint qHash(const Pointer &node) { return ::qHash(node.data()); }

inline bool operator==(const Reference &a, const Reference &b) { return a->isEqualTo(b); }
inline bool operator!=(const Reference &a, const Reference &b) { return !a->isEqualTo(b); }
inline uint qHash(const Reference &node) { return node->hash(); }

#define CHILD_DECLARE(NAME, ORIGIN) \
public: \
    static NAME##Pointer &root(); \
    virtual const QString className() const { return #NAME; } \
    static const bool isInitialized; \
    static const NAME##Pointer &empty() { static const NAME##Pointer _empty(new NAME(NAME::root())); return _empty; }; \
private:

#define CHILD_DEFINE(NAME, ORIGIN) \
const bool NAME::isInitialized = NAME::root().isNotNull(); \
NAME##Pointer &NAME::root() { \
    static NAME##Pointer _root; \
    if(!_root) { \
        _root = NAME##Pointer(new NAME(ORIGIN::root())); \
        empty(); \
        initRoot(); \
    } \
    return _root; \
}

CHILD_END

#endif // CHILD_NODE_H
