#ifndef CHILD_NODE_TEST_H
#define CHILD_NODE_TEST_H

#include "child/node.h"

#include "tests/testbegin.h"

class NodeTest : public QObject {
    Q_OBJECT
private:
    HugeUnsignedInteger _initialNodeCount;
private slots:
    void init() { _initialNodeCount = Node::nodeCount(); }
    void cleanup() { QVERIFY(Node::nodeCount() == _initialNodeCount); }

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

#include "tests/testend.h"

#endif // CHILD_NODE_TEST_H
