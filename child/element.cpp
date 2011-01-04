#include "child/element.h"

CHILD_BEGIN

CHILD_DEFINE(Element, Object);

void Element::initRoot() {
    Object::root()->addChild("Element", root());
}

CHILD_END
