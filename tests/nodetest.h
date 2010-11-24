#ifndef CHILD_NODETEST_H
#define CHILD_NODETEST_H

#include "tests/testhelper.h"
#include "child/node.h"

namespace Child {
    class NodeTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialNodeCount;
    private slots:
        void initTestCase() {
            Node::root();
        }

        void init() {
            _initialNodeCount = Node::nodeCount();
        }

        void cleanup() {
            QVERIFY(Node::nodeCount() == _initialNodeCount);
        }

        void initialize();
        void setOrigin();
        void unsetOrigin();
        void directOriginIs();
        void fork();
        void addExtension();
        void prependExtension();
        void removeExtension();
        void hasExtension();
        void addParent();
        void removeParent();
        void directChild();
        void hasDirectChild();
        void addDirectChild();
        void setDirectChild();
        void removeDirectChild();
        void child();
        void setChild();
        void inspect();
    };
}

#endif // CHILD_NODETEST_H
