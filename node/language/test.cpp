#include "child/language/test.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Test, Element);

    void Test::initRoot() {
        Language::root()->addChild("Test", this);
    }
}

CHILD_END
