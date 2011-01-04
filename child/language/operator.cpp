#include "child/language/operator.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Operator, Object);

    void Operator::initRoot() {
        Language::root()->addChild("Operator", root());
    }
}

CHILD_END
