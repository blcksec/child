#include "node/object/boolean.h"

CHILD_BEGIN

CHILD_DEFINE(Boolean, Element, Object);

void Boolean::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Boolean, equal_to, ==);
}

CHILD_END
