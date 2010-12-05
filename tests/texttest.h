#ifndef CHILD_TEXTTEST_H
#define CHILD_TEXTTEST_H

#include "tests/testhelper.h"
#include "child/text.h"

#ifdef CHILD_IS_NAMESPACED
namespace Child {
#endif

class TextTest : public QObject
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
    void fork();
    void compare();
    void upcase();
};

#ifdef CHILD_IS_NAMESPACED
}
#endif

#endif // CHILD_TEXTTEST_H
