#include "child/language/primitive.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Primitive, Element);

    void Primitive::initRoot() {
        Language::root()->addChild("Primitive", root());
    }
}

CHILD_END
