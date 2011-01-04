#include "child/boolean.h"

CHILD_BEGIN

CHILD_DEFINE(Boolean, Element);

void Boolean::initRoot() {
    Object::root()->addChild("Boolean", root());
    CHILD_NATIVE_METHOD_ADD(Boolean, equal_to, ==);
}

CHILD_END
