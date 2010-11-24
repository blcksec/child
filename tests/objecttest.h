#ifndef CHILD_OBJECTTEST_H
#define CHILD_OBJECTTEST_H

#include "tests/testhelper.h"
#include "child/object.h"

namespace Child {
    class ObjectTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialNodeCount;
    private slots:
        void initTestCase() {
            Object::root();
        }

        void init() {
            _initialNodeCount = Node::nodeCount();
        }

        void cleanup() {
            QVERIFY(Node::nodeCount() == _initialNodeCount);
        }

        void initialize();
    };
}

#endif // CHILD_OBJECTTEST_H
