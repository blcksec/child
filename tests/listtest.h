#ifndef CHILD_LISTTEST_H
#define CHILD_LISTTEST_H

#include "tests/testhelper.h"
#include "child/text.h"
#include "child/list.h"
#include "child/world.h"

namespace Child {
    class ListTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialNodeCount;
    private slots:
        void initTestCase() {
            List::root();
        }

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
}

#endif // CHILD_LISTTEST_H
