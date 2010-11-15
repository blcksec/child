#ifndef CHILD_NODE_H
#define CHILD_NODE_H

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QQueue>
#include <QtCore/QStack>

#include "child/toolbox.h"
#include "child/exception.h"

namespace Child {
    class Node;

    class NamedNode {
    public:
        NamedNode() { throw(RuntimeException("cannot construct a NULL NamedNode")); }
        NamedNode(const QString &t, Node *const n);
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
    typedef QHash<QString, Node *> NodeHash;
    typedef QHashIterator<QString, Node *> NodeHashIterator;
    typedef QMultiHash<QString, Node *> NodeMultiHash;
    typedef QSet<Node *> NodeSet;
    typedef QQueue<Node *> NodeQueue;
    typedef QList<NamedNode> NamedNodeList;
    typedef QSet<NamedNode> NamedNodeSet;
    typedef Node *(Node::*NodeMethodPtr)();

    class Node {
    public:
        Node() :
            _origin(NULL), _forks(NULL), _extensions(NULL), _extendedNodes(NULL),
            _parents(NULL), _children(NULL), _isVirtual(false) {
            _nodeCount++;
        }
        virtual ~Node();
        static const long long int nodeCount() { return(_nodeCount); }

        static Node *root();

        bool isVirtual() const { return(_isVirtual); }
        Node *setIsVirtual(bool value) { _isVirtual = value; return(this); }

        Node *origin() const { return(_origin); }
        void setOrigin(Node *node);
        void unsetOrigin() { if(_origin) { _origin->_forks->removeOne(this); _origin = NULL; } }
        NodeList forks() const { return(_forks ? NodeList(*_forks) : NodeList()); }
        bool directOriginIs(Node *node) const;
    protected:
        template<typename T> static T *_fork(T *node) {
            T *fork = new T;
            fork->setOrigin(node);
            return(fork);
        }
    public:
        virtual Node *fork() { return(_fork(this)); }
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
        const QString inspect() const;
    private:
        static long long int _nodeCount;
        static Node *_root;
        Node *_origin;
        NodeList *_forks; // backlink cache
        NodeList *_extensions;
        NodeList *_extendedNodes; // backlink cache
        NamedNodeSet *_parents;
        NodeHash *_children; // backlink cache
        bool _isVirtual : 1;
    };
}

#endif // CHILD_NODE_H
