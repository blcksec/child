#ifndef CHILD_OBJECT_TEST_H
#define CHILD_OBJECT_TEST_H

#include "child/object.h"

#include "tests/testbegin.h"

class ObjectTest : public QObject {
    Q_OBJECT
private:
    HugeUnsignedInteger _initialNodeCount;
private slots:
    void init() { _initialNodeCount = Node::nodeCount(); }
    void cleanup() { QVERIFY(Node::nodeCount() == _initialNodeCount); }

    void initialize();
};

#include "tests/testend.h"

#endif // CHILD_OBJECT_TEST_H
