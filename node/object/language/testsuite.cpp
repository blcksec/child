#include "node/object/language/testsuite.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(TestSuite, List, Language);

    void TestSuite::initRoot() {
    }

    Node *TestSuite::run(Node *receiver) {
        Q_UNUSED(receiver);
        CHILD_PUSH_RUN(this);
        Iterator i(this);
        while(Test *test = i.next())
            test->run();
        return this;
    }
}

CHILD_END
