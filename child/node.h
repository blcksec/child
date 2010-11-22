#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QQueue>
#include <QtCore/QStack>

#include "child/toolbox.h"

#define CHILD_DECLARATION(NAME) \
public: \
    static const QString &className() { return(NAME::_className); } \
    static NAME *root(); \
    static void initRoot(); \
    static NAME *fork(Node *world) { return(static_cast<NAME *>(world->child(#NAME))->fork()); } \
    virtual NAME *fork() { \
        NAME *f = new NAME; \
        f->setOrigin(this); \
        f->initFork(); \
        return(f); \
    } \
    static NAME *as(Node *node) { return(static_cast<NAME *>(node)); } \
    static NAME *is(Node *node) { return(dynamic_cast<NAME *>(node)); } \
private: \
    static QString _className; \
    static NAME *_root;

#define CHILD_IMPLEMENTATION(NAME, PARENT) \
QString NAME::_className = #NAME; \
NAME *NAME::_root = NAME::root(); \
NAME *NAME::root() { \
    if(!_root) { \
        _root = new NAME; \
        _root->setOrigin(PARENT::root()); \
        _root->addParent(#NAME, PARENT::root()); \
        NAME::initRoot(); \
    } \
    return(_root); \
}

namespace Child {
    class Node;

    class NumberedNode {
    public:
        NumberedNode() { throw(RuntimeException("cannot construct a NULL NumberedNode")); }
        NumberedNode(const int number, Node *const node);
        int number;
        Node *node;
    };

    inline bool operator==(const NumberedNode &a, const NumberedNode &b) {
        return(a.node == b.node && a.number == b.number);
    }

    inline uint qHash(const NumberedNode &key) {
        return ::qHash(key.number) ^ ::qHash(key.node);
    }

    class NamedNode {
    public:
        NamedNode() { throw(RuntimeException("cannot construct a NULL NamedNode")); }
        NamedNode(const QString &name, Node *const node);
        QString name;
        Node *node;
    };

    inline bool operator==(const NamedNode &a, const NamedNode &b) {
        return(a.node == b.node && a.name == b.name);
    }

    inline uint qHash(const NamedNode &key) {
        return qHash(key.name) ^ ::qHash(key.node);
    }

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

    class Node {
        CHILD_DECLARATION(Node);
    public:
        static const long long int nodeCount() { return(_nodeCount); }

        Node() :
            _origin(NULL), _forks(NULL), _extensions(NULL), _extendedNodes(NULL),
            _parents(NULL), _children(NULL), _isVirtual(false) {
            _nodeCount++;
        }

        virtual ~Node();

        virtual void initFork() {}

        bool isVirtual() const { return(_isVirtual); }
        Node *setIsVirtual(bool value) { _isVirtual = value; return(this); }

        Node *origin() const { return(_origin); }
        Node *setOrigin(Node *node);
        void unsetOrigin() { if(_origin) { _origin->_forks->removeOne(this); _origin = NULL; } }
        NodeList forks() const { return(_forks ? NodeList(*_forks) : NodeList()); }
        bool directOriginIs(Node *node) const;
        void removeAllForks();

        NodeList extensions() const { return(_extensions ? NodeList(*_extensions) : NodeList()); }
        NodeList extendedNodes() const { return(_extendedNodes ? NodeList(*_extendedNodes) : NodeList()); }
        bool hasExtension(Node *node) const;
        void addExtension(Node *node);
        void prependExtension(Node *node);
        void removeExtension(Node *node);
        void removeAllExtensions();
        void removeAllExtendedNodes();

        const NodeMultiHash parents() const;
        const NodeHash children() const { return(_children ? NodeHash(*_children) : NodeHash()); }
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

        const long long int uniqueID() const { return(reinterpret_cast<long long int>(this)); }
        const QString uniqueHexID() const { return(QString("0x%1").arg(uniqueID(), 0, 16)); }
        virtual const QString inspect() const;
    private:
        static long long int _nodeCount;
        Node *_origin;
        NodeList *_forks; // backlink cache
        NodeList *_extensions;
        NodeList *_extendedNodes; // backlink cache
        NamedNodeSet *_parents;
        NodeHash *_children; // backlink cache
        bool _isVirtual : 1;

        void checkName(const QString &name) const {
            if(name.isEmpty()) throw(ArgumentException("child name is empty"));
        }

        void checkNode(Node *node) const {
            if(!node) throw(NullPointerException("Node pointer is NULL"));
        }
    };
}

#endif // CHILD_NODE_H
