#ifndef CHILD_OBJECTTEST_H
#define CHILD_OBJECTTEST_H

#include "tests/testhelper.h"
#include "child/object.h"

#ifdef CHILD_IS_NAMESPACED
namespace Child {
#endif

class ObjectTest : public QObject
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
};

#ifdef CHILD_IS_NAMESPACED
}
#endif

#endif // CHILD_OBJECTTEST_H
