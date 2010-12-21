#ifndef CHILD_TEXT_TEST_H
#define CHILD_TEXT_TEST_H

#include "child/text.h"

#include "tests/testbegin.h"

class TextTest : public QObject {
    Q_OBJECT
private:
    HugeUnsignedInteger _initialNodeCount;
private slots:
    void init() { _initialNodeCount = Node::nodeCount(); }
    void cleanup() { QVERIFY(Node::nodeCount() == _initialNodeCount); }

    void initialize();
    void fork();
    void compare();
};

#include "tests/testend.h"

#endif // CHILD_TEXT_TEST_H
