#include "node/object/boolean.h"

CHILD_BEGIN

CHILD_DEFINE(Boolean, Element, Object);

void Boolean::initRoot() {
    CHILD_NATIVE_METHOD_ADD(Boolean, equal_to, ==);
}

CHILD_END
