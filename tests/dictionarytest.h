#ifndef CHILD_DICTIONARYTEST_H
#define CHILD_DICTIONARYTEST_H

#include "tests/testhelper.h"
#include "child/text.h"
#include "child/dictionary.h"
#include "child/world.h"

namespace Child {
    class DictionaryTest : public QObject
    {
        Q_OBJECT
    private:
        long long int _initialNodeCount;
    private slots:
        void initTestCase() {
            Dictionary::root();
        }

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
}

#endif // CHILD_DICTIONARYTEST_H
