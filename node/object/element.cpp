#include "node/element.h"

CHILD_BEGIN

CHILD_DEFINE(Element, Object);

void Element::initRoot() {
    Object::root()->addChild("Element", this);
}

CHILD_END
