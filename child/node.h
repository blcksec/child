#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QHash>
#include <QtCore/QStack>

#include "child/global.h"
#include "child/pointer.h"

CHILD_BEGIN

namespace Language { class ArgumentBunchPointer; }
using namespace Language;

#define CHILD_NATIVE_METHOD_DECLARE(METHOD) \
Pointer _##METHOD##_(const ArgumentBunchPointer &inputs)

#define CHILD_NODE(ARGS...) Pointer(new Node(Node::context()->child("Node"), ##ARGS))

#define CHILD_CHECK_POINTER(POINTER) \
if(!(POINTER)) CHILD_THROW_NULL_POINTER_EXCEPTION("Pointer is NULL")

typedef QList<Pointer> PointerList;
typedef QHash<Reference, Pointer> ReferenceHash;
typedef Pointer (Node::*_MethodPointer_)(const ArgumentBunchPointer &);

class Node {
public:
    friend class GenericPointer<Node>;

    enum Comparison { Smaller = -2, SmallerOrEqual, Equal, GreaterOrEqual, Greater, Different };

    static const bool isInitialized;

    Node(const Pointer &origin) : _origin(origin), _extensions(NULL), // default constructor
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

    const Pointer child(const QString &name) const;

    const Pointer child(const QString &name1, const QString &name2) const {
        return child(name1)->child(name2);
    }

    const Pointer child(const QString &name1, const QString &name2, const QString &name3) const {
        return child(name1)->child(name2)->child(name3);
    }

    const Pointer addChild(const QString &name, const Pointer &value);
    const Pointer setChild(const QString &name, const Pointer &value);
    void removeChild(const QString &name);

    void addAnonymousChild(const Pointer &value) {
        CHILD_CHECK_POINTER(value);
        value->_addParent(this);
    }

    void removeAnonymousChild(const Pointer &value) {
        CHILD_CHECK_POINTER(value);
        value->_removeParent(this);
    }

    const Pointer hasChild(const QString &name, bool searchInParents = true,
                           bool forkChildFoundInFirstOrigin = true, bool *isDirectPointer = NULL) const;

    const Pointer hasDirectChild(const QString &name, bool *isRemovedPointer = NULL) const {
        Pointer child;
        if(_children) child = _children->value(name);
        if(isRemovedPointer) *isRemovedPointer = !child && _children && _children->contains(name);
        return child;
    }

    const QString hasDirectChild(const Pointer &value) const {
        return _children ? _children->key(value) : QString();
    }

    bool hasDirectParent(const Pointer &parent) const {
        CHILD_CHECK_POINTER(parent);
        return _parents && _parents->contains(parent.data());
    }

    const QHash<QString, Pointer> children() const;
    const PointerList parents() const;

    virtual Pointer run(const Pointer &receiver = context()) {
        Q_UNUSED(receiver);
        return this;
    }

    Pointer print() const { P(toString().toUtf8()); return this; }
    CHILD_NATIVE_METHOD_DECLARE(print) { Q_UNUSED(inputs); return print(); }

    const long long int memoryAddress() const { return reinterpret_cast<long long int>(this); }
    const QString hexMemoryAddress() const { return QString("0x%1").arg(memoryAddress(), 0, 16); }
    virtual const QString toString(bool debug = false, short level = 0) const;
    Pointer inspect() const { P(toString(true).toUtf8()); return this; }
    CHILD_NATIVE_METHOD_DECLARE(inspect) { Q_UNUSED(inputs); return inspect(); }

    virtual Comparison compare(const Node &other) const { return this == &other ? Equal : Different; }
    virtual uint hash() const { return ::qHash(this); }

    static const Pointer &empty() { static const Pointer _empty(new Node(Node::root())); return _empty; };

    static HugeUnsignedInteger &nodeCount() {
        static HugeUnsignedInteger _nodeCount = 0;
        return _nodeCount;
    }

    static const Pointer context() {
        if(contextStack().isEmpty())
            qFatal("Fatal error: context stack is empty!");
        return contextStack().top();
    }

    static void pushContext(const Pointer &node) { contextStack().push(node); }

    static const Pointer popContext() {
        if(contextStack().isEmpty()) qFatal("Fatal error: context stack is empty!");
        return contextStack().pop();
    }

    static QStack<Pointer> &contextStack() {
        static QStack<Pointer> _contextStack;
        return _contextStack;
    }
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

inline bool operator==(const Node &a, const Node &b) { return a.compare(b) == Node::Equal; }
inline bool operator!=(const Node &a, const Node &b) { return a.compare(b) != Node::Equal; }
inline uint qHash(const Node &node) { return node.hash(); }

inline bool operator==(const Pointer &a, const Pointer &b) { return a.data() == b.data(); }
inline bool operator!=(const Pointer &a, const Pointer &b) { return a.data() != b.data(); }
inline uint qHash(const Pointer &node) { return ::qHash(node.data()); }

inline bool operator==(const Reference &a, const Reference &b) { return a->compare(*b) == Node::Equal; }
inline bool operator!=(const Reference &a, const Reference &b) { return a->compare(*b) != Node::Equal; }
inline uint qHash(const Reference &node) { return node->hash(); }

#define CHILD_DECLARATION(NAME, ORIGIN) \
public: \
    static NAME##Pointer &root(); \
    virtual const QString className() const { return #NAME; } \
    static const bool isInitialized; \
    static const NAME##Pointer &empty() { static const NAME##Pointer _empty(new NAME(NAME::root())); return _empty; }; \
private:

#define CHILD_DEFINITION(NAME, ORIGIN) \
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
