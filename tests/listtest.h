#ifndef CHILD_LISTTEST_H
#define CHILD_LISTTEST_H

#include "tests/testhelper.h"
#include "child/text.h"
#include "child/list.h"

#ifdef CHILD_IS_NAMESPACED
namespace Child {
#endif

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
        QVERIFY(Node::nodeCount() == _initialNodeCount);
    }

    void initialize();
    void insertGetAndSet();
    void remove();
    void clear();
};

#ifdef CHILD_IS_NAMESPACED
}
#endif

#endif // CHILD_LISTTEST_H
