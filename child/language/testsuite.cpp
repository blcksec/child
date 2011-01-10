#include "child/language/testsuite.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(TestSuite, List);

    void TestSuite::initRoot() {
        Language::root()->addChild("TestSuite", this);
    }
}

CHILD_END
