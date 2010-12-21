#ifndef CHILD_LIST_TEST_H
#define CHILD_LIST_TEST_H

#include "child/text.h"
#include "child/list.h"

#include "tests/testbegin.h"

class ListTest : public QObject {
    Q_OBJECT
private:
    HugeUnsignedInteger _initialNodeCount;
private slots:
    void init() { _initialNodeCount = Node::nodeCount(); }
    void cleanup() { QVERIFY(Node::nodeCount() == _initialNodeCount); }

    void initialize();
    void insertGetAndSet();
    void remove();
    void clear();
};

#include "tests/testend.h"

#endif // CHILD_LIST_TEST_H
