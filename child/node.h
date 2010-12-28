#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QHash>
#include <QtCore/QStack>

#include "child/global.h"

CHILD_BEGIN

class Message;

#define CHILD_NATIVE_METHOD_DECLARE(METHOD) \
Node *_##METHOD##_(Message *message)

#define CHILD_NATIVE_METHOD_DEFINE(NAME, METHOD) \
Node *NAME::_##METHOD##_(Message *message)

#define CHILD_NODE(ARGS...) new Node(Node::context()->child("Node"), ##ARGS)

#define CHILD_CHECK_POINTER(POINTER) \
if(!(POINTER)) CHILD_THROW_NULL_POINTER_EXCEPTION("Node pointer is NULL")

class Node : public gc_cleanup {
public:
    static const bool isInitialized;

    explicit Node(Node *origin) : _origin(origin), _extensions(NULL), // default constructor
        _children(NULL), _parents(NULL) {}

    Node(const Node &other) : gc_cleanup(), _origin(other._origin), _extensions(NULL), // copy constructor
        _children(NULL), _parents(NULL) {
        if(other._extensions) _extensions = new QList<Node *>(*other._extensions);
        if(other._children) {
            QHashIterator<QString, Node *> i(other.children());
            while(i.hasNext()) { i.next(); addChild(i.key(), i.value()); }
        }
    }

    virtual ~Node();

    static Node *constCast(const Node *node) { return const_cast<Node *>(node); }

    static Node *root();
    virtual const QString className() const { return "Node"; }
    static void initRoot();
    virtual Node *fork() const { return new Node(constCast(this)); }
    static Node *forkIfNotNull(Node *node) { return node ? node->fork() : node; }

    Node *origin() const { return _origin; }
    void setOrigin(Node *node);

    QList<Node *> extensions() const { return _extensions ? *_extensions : QList<Node *>(); }

    void addExtension(Node *node);
    void prependExtension(Node *node);
    void removeExtension(Node *node);
    void removeAllExtensions();
    bool hasExtension(Node *node) const;

    Node *child(const QString &name) const;

    Node *child(const QString &name1, const QString &name2) const {
        return child(name1)->child(name2);
    }

    Node *child(const QString &name1, const QString &name2, const QString &name3) const {
        return child(name1)->child(name2)->child(name3);
    }

    Node *addChild(const QString &name, Node *value);
    Node *setChild(const QString &name, Node *value, bool addOrSetMode = false);

    Node *addOrSetChild(const QString &name, Node *value) {
        return setChild(name, value, true);
    }

    void removeChild(const QString &name);

    void addAnonymousChild(Node *value) {
        CHILD_CHECK_POINTER(value);
        value->_addParent(this);
    }

    void removeAnonymousChild(Node *value) {
        CHILD_CHECK_POINTER(value);
        value->_removeParent(this);
    }

private:
    void _setChild(const QString &name, Node *value);
public:

    Node *hasChild(const QString &name, bool searchInParents = true,
                           bool forkChildFoundInFirstOrigin = true, bool *isDirectPtr = NULL) const;

    Node *hasDirectChild(const QString &name, bool *isRemovedPtr = NULL) const {
        Node *child = NULL;
        if(_children) child = _children->value(name);
        if(isRemovedPtr) *isRemovedPtr = !child && _children && _children->contains(name);
        return child;
    }

    QString hasDirectChild(Node *value) const {
        return _children ? _children->key(value) : QString();
    }

    bool hasDirectParent(Node *parent) const {
        CHILD_CHECK_POINTER(parent);
        return _parents && _parents->contains(parent);
    }

private:
    void _addParent(Node *parent) const;
    void _removeParent(Node *parent) const;
public:
    QHash<QString, Node *> children() const;
    QList<Node *> parents() const;

    virtual Node *run(Node *receiver = context()) {
        Q_UNUSED(receiver);
        return this;
    }

//    virtual Node *run(Node *receiver, Message *message);

//    CHILD_NATIVE_METHOD_DECLARE(fork);
//private:
//    Node *defineOrAssign(Message *message, bool isDefine);
//public:
//    CHILD_NATIVE_METHOD_DECLARE(define) { return(defineOrAssign(message, true)); }
//    CHILD_NATIVE_METHOD_DECLARE(assign) { return(defineOrAssign(message, false)); }

//    virtual void hasBeenAssigned(Message *message) const {
//        Q_UNUSED(message);
//    }

//    CHILD_NATIVE_METHOD_DECLARE(or);
//    CHILD_NATIVE_METHOD_DECLARE(and);
//    CHILD_NATIVE_METHOD_DECLARE(not);

//    CHILD_NATIVE_METHOD_DECLARE(or_assign);
//    CHILD_NATIVE_METHOD_DECLARE(and_assign);

    virtual bool isEqualTo(const Node *other) const { return this == other; }
//    CHILD_NATIVE_METHOD_DECLARE(equal_to);
//    CHILD_NATIVE_METHOD_DECLARE(different_from);

//    CHILD_NATIVE_METHOD_DECLARE(assert);

    void print() const { P(toString().toUtf8()); }
//    CHILD_NATIVE_METHOD_DECLARE(print);

    void inspect() const { P(toString(true).toUtf8()); }
//    CHILD_NATIVE_METHOD_DECLARE(inspect);

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

    static Node *context() {
        if(contextStack().isEmpty())
            qFatal("Fatal error: context stack is empty!");
        return contextStack().top();
    }

    static void pushContext(Node *node) { contextStack().push(node); }

    static Node *popContext() {
        if(contextStack().isEmpty()) qFatal("Fatal error: context stack is empty!");
        return contextStack().pop();
    }

    static QStack<Node *> &contextStack() {
        static QStack<Node *> _contextStack;
        return _contextStack;
    }

    class ContextPusher {
    public:
        explicit ContextPusher(Node *node) { pushContext(node); }
        ~ContextPusher() { popContext(); }
    private:
        ContextPusher(const ContextPusher &); // prevent copying
        ContextPusher &operator=(const ContextPusher &);
    };
private:
    Node *_origin;
    QList<Node *> *_extensions;
    QHash<QString, Node *> *_children;
    mutable QHash<Node *, HugeUnsignedInteger> *_parents;
};

inline bool operator==(const Node &a, const Node &b) { return a.isEqualTo(&b); }
inline bool operator!=(const Node &a, const Node &b) { return !a.isEqualTo(&b); }
inline uint qHash(const Node &node) { return node.hash(); }

//inline bool operator==(const Reference &a, const Reference &b) { return a->isEqualTo(b); }
//inline bool operator!=(const Reference &a, const Reference &b) { return !a->isEqualTo(b); }
//inline uint qHash(const Reference &node) { return node->hash(); }

#define CHILD_DECLARE(NAME, ORIGIN) \
public: \
    static NAME *cast(Node *node) { return static_cast<NAME *>(node); } \
    static NAME *dynamicCast(Node *node) { return dynamic_cast<NAME *>(node); } \
    static NAME *constCast(const NAME *node) { return const_cast<NAME *>(node); } \
    static NAME *root(); \
    virtual const QString className() const { return #NAME; } \
    static const bool isInitialized; \
private:

#define CHILD_DEFINE(NAME, ORIGIN) \
const bool NAME::isInitialized = NAME::root(); \
NAME *NAME::root() { \
    static NAME *_root = NULL; \
    if(!_root) { \
        _root = new NAME(ORIGIN::root()); \
        initRoot(); \
    } \
    return _root; \
}

CHILD_END

#endif // CHILD_NODE_H
