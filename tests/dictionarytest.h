#ifndef CHILD_DICTIONARYTEST_H
#define CHILD_DICTIONARYTEST_H

#include "tests/testhelper.h"
#include "child/text.h"
#include "child/dictionary.h"
#include "child/list.h"

#ifdef CHILD_IS_NAMESPACED
namespace Child {
#endif

class DictionaryTest : public QObject
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
    void getAndSet();
    void keys();
    void remove();
    void clear();
};

#ifdef CHILD_IS_NAMESPACED
}
#endif

#endif // CHILD_DICTIONARYTEST_H
