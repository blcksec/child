#include "child/method.h"

CHILD_BEGIN

CHILD_DEFINE(Method, Element);

void Method::initRoot() {
    Object::root()->addChild("Method", this);
}

CHILD_END
