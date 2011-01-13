#include "node/property.h"

CHILD_BEGIN

CHILD_DEFINE(Property, Object);

void Property::initRoot() {
    Object::root()->addChild("Property", this);
}

CHILD_END
