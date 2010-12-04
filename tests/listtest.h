#ifndef CHILD_LISTTEST_H
#define CHILD_LISTTEST_H

#include "tests/testhelper.h"
#include "child/text.h"
#include "child/list.h"

namespace Child {

class ListTest : public QObject
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
    void insertGetAndSet();
    void remove();
    void clear();
};

} // namespace Child

#endif // CHILD_LISTTEST_H
