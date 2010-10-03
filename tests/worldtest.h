#ifndef WORLDTEST_H
#define WORLDTEST_H

#include "tests/testhelper.h"
#include "child/world.h"

namespace Child {
    class WorldTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialNodeCount;
    private slots:
        void init() {
            _initialNodeCount = Node::nodeCount();
        }

        void cleanup() {
            QVERIFY(Node::nodeCount() == _initialNodeCount);
        }

        void new_();
    };
}

#endif // WORLDTEST_H
