#include "child/language/testsuite.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(TestSuite, List);

    void TestSuite::initRoot() {
        Language::root()->addChild("TestSuite", this);
    }

    Node *TestSuite::run(Node *receiver) {
        Iterator i(this);
        while(Test *test = i.next())
            test->run(receiver);
        return this;
    }
}

CHILD_END
