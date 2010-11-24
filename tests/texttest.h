#ifndef CHILD_TEXTTEST_H
#define CHILD_TEXTTEST_H

#include "tests/testhelper.h"
#include "child/text.h"
#include "child/world.h"

namespace Child {
    class TextTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialNodeCount;
    private slots:
        void initTestCase() {
            Text::root();
        }

        void init() {
            _initialNodeCount = Node::nodeCount();
        }

        void cleanup() {
            QVERIFY(Node::nodeCount() == _initialNodeCount);
        }

        void initialize();
        void fork();
    };
}

#endif // CHILD_TEXTTEST_H
