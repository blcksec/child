#ifndef CHILD_NODETEST_H
#define CHILD_NODETEST_H

#include "tests/testhelper.h"
#include "child/node.h"

namespace Child {

class NodeTest : public QObject
{
    Q_OBJECT
private:
    HugeUnsignedInteger _initialNodeCount;
private slots:
    void init() {
        _initialNodeCount = Node::nodeCount();
    }

    void cleanup() {
        QCOMPARE(Node::nodeCount(), _initialNodeCount);
    }

    void initialize();
    void setOrigin();
    void fork();
    void addExtension();
    void removeExtension();
    void hasExtension();
    void addChild();
    void setChild();
    void removeChild();
    void hasDirectChild();
    void hasChild();
};

} // namespace Child

#endif // CHILD_NODETEST_H
