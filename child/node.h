#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QHash>
#include <QtCore/QStack>

#include "child/global.h"

CHILD_BEGIN

#define CHILD_NODE(ARGS...) new Node(Node::context()->child("Node"), ##ARGS)

#define CHILD_FORK_METHOD(NAME, ARGS...) \
virtual NAME *fork() const { \
    NAME *node = new NAME(constCast(this), ##ARGS); \
    node->initFork(); \
    return node; \
}

#define CHILD_FORK_IF_NOT_NULL(NODE) \
((NODE) ? (NODE)->fork() : NULL)

#define CHILD_CHECK_POINTER(POINTER) \
if(!(POINTER)) CHILD_THROW_NULL_POINTER_EXCEPTION("Node pointer is NULL")

class Message;

#define CHILD_NATIVE_METHOD_DECLARE(METHOD) \
Node *_##METHOD##_(Message *message)

#define CHILD_NATIVE_METHOD_DEFINE(NAME, METHOD) \
Node *NAME::_##METHOD##_(Message *message)

namespace Language {
    class Primitive;
}

class Node {
public:
    static const bool isInitialized;

    explicit Node(Node *origin) : _origin(origin), _extensions(NULL), // default constructor
        _children(NULL), _parents(NULL) {}

    Node(const Node &other) : _origin(other._origin), _extensions(NULL), // copy constructor
        _children(NULL), _parents(NULL) {
        if(other._extensions) _extensions = new QList<Node *>(*other._extensions);
        if(other._children) {
            QHashIterator<QString, const Node *> i(other.children());
            while(i.hasNext()) { i.next(); addChild(i.key(), constCast(i.value())); }
        }
    }

    virtual ~Node();

    inline static Node *constCast(const Node *node) { return const_cast<Node *>(node); }

    static Node *root();
    virtual const QString className() const { return "Node"; }
    static void initRoot();

    CHILD_FORK_METHOD(Node);
    virtual void initFork() {}

    Node *origin() { return _origin; }
    const Node *origin() const { return _origin; }
    void setOrigin(Node *node);

    void addExtension(Node *node);
    void prependExtension(Node *node);
    void removeExtension(const Node *node);
    void removeAllExtensions();
    bool hasExtension(const Node *node) const;
    QList<Node *> extensions();
    QList<const Node *> extensions() const;

    Node *child(const QString &name);

    const Node *child(const QString &name) const {
        return constCast(this)->child(name);
    }

    Node *child(const QString &name1, const QString &name2) {
        return child(name1)->child(name2);
    }

    const Node *child(const QString &name1, const QString &name2) const {
        return constCast(this)->child(name1)->child(name2);
    }

    Node *child(const QString &name1, const QString &name2, const QString &name3) {
        return child(name1)->child(name2)->child(name3);
    }

    const Node *child(const QString &name1, const QString &name2, const QString &name3) const {
        return constCast(this)->child(name1)->child(name2)->child(name3);
    }

    void addChild(const QString &name, Node *value);
    void setChild(const QString &name, Node *value, bool addOrSetMode = false);
    void addOrSetChild(const QString &name, Node *value) { setChild(name, value, true); }

    void removeChild(const QString &name);

    void addAnonymousChild(const Node *value) const {
        CHILD_CHECK_POINTER(value);
        value->_addParent(this);
    }

    void removeAnonymousChild(const Node *value) const {
        CHILD_CHECK_POINTER(value);
        value->_removeParent(this);
    }

private:
    void _setChild(const QString &name, Node *value);
public:

    Node *hasChild(const QString &name, bool searchInParents = true,
                   bool autoFork = true, bool *isDirectPtr = NULL);

    const Node *hasChild(const QString &name, bool searchInParents = true,
                         bool autoFork = true, bool *isDirectPtr = NULL) const {
        return constCast(this)->hasChild(name, searchInParents, autoFork, isDirectPtr);
    }

    Node *hasDirectChild(const QString &name, bool *isRemovedPtr = NULL) {
        Node *child = NULL;
        if(_children) child = constCast(_children->value(name));
        if(isRemovedPtr) *isRemovedPtr = !child && _children && _children->contains(name);
        return child;
    }

    const Node *hasDirectChild(const QString &name, bool *isRemovedPtr = NULL) const {
        return constCast(this)->hasDirectChild(name, isRemovedPtr);
    }

    QString hasDirectChild(const Node *value) const {
        return _children ? _children->key(constCast(value)) : QString();
    }

    bool hasDirectParent(const Node *parent) const {
        CHILD_CHECK_POINTER(parent);
        return _parents && _parents->contains(parent);
    }

private:
    void _addParent(const Node *parent) const;
    void _removeParent(const Node *parent) const;
public:

    QHash<QString, Node *> children();
    QHash<QString, const Node *> children() const;
    QList<Node *> parents();
    QList<const Node *> parents() const;

    virtual Node *receive(Language::Primitive *primitive);

    virtual Node *run(Node *receiver = context()) {
        Q_UNUSED(receiver);
        return this;
    }

    virtual Node *run(Node *receiver, Message *message);

    CHILD_NATIVE_METHOD_DECLARE(fork);
private:
    Node *defineOrAssign(Message *message, bool isDefine);
public:
    CHILD_NATIVE_METHOD_DECLARE(define) { return defineOrAssign(message, true); }
    CHILD_NATIVE_METHOD_DECLARE(assign) { return defineOrAssign(message, false); }

    virtual void hasBeenAssigned(Message *message) const {
        Q_UNUSED(message);
    }

    CHILD_NATIVE_METHOD_DECLARE(or);
    CHILD_NATIVE_METHOD_DECLARE(and);
    CHILD_NATIVE_METHOD_DECLARE(not);

    CHILD_NATIVE_METHOD_DECLARE(or_assign);
    CHILD_NATIVE_METHOD_DECLARE(and_assign);

    virtual bool isEqualTo(const Node *other) const { return this == other; }
    CHILD_NATIVE_METHOD_DECLARE(equal_to);
    CHILD_NATIVE_METHOD_DECLARE(different_from);

    CHILD_NATIVE_METHOD_DECLARE(assert);

    void print() const { P(toString().toUtf8()); }
    CHILD_NATIVE_METHOD_DECLARE(print);

    void inspect() const { P(toString(true).toUtf8()); }
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

     class Reference {
     public:
         Reference(Node *node) : _node(node) {}
         Node &operator*() { return *_node; }
         const Node &operator*() const { return *_node; }
         Node *operator->() { return _node; }
         const Node *operator->() const { return _node; }
         operator bool() const { return _node; }
         bool operator!() const { return !_node; }
         operator Node *() const { return _node; }
     private:
         Node *_node;
     };
private:
    Node *_origin;
    QList<Node *> *_extensions;
    QHash<QString, Node *> *_children;
    mutable QHash<const Node *, HugeUnsignedInteger> *_parents;
};

inline bool operator==(const Node &a, const Node &b) { return a.isEqualTo(&b); }
inline bool operator!=(const Node &a, const Node &b) { return !a.isEqualTo(&b); }
inline uint qHash(const Node &node) { return node.hash(); }

inline bool operator==(const Node::Reference &a, const Node::Reference &b) { return a->isEqualTo(b); }
inline bool operator!=(const Node::Reference &a, const Node::Reference &b) { return !a->isEqualTo(b); }
inline uint qHash(const Node::Reference &node) { return node->hash(); }

#define CHILD_DECLARE(NAME, ORIGIN) \
public: \
    inline static NAME *cast(Node *node) { return static_cast<NAME *>(node); } \
    inline static const NAME *cast(const Node *node) { return static_cast<const NAME *>(node); } \
    inline static NAME *dynamicCast(Node *node) { return dynamic_cast<NAME *>(node); } \
    inline static const NAME *dynamicCast(const Node *node) { return dynamic_cast<const NAME *>(node); } \
    inline static NAME *constCast(const NAME *node) { return const_cast<NAME *>(node); } \
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
