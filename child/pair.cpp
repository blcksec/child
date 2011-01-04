#include "child/pair.h"

CHILD_BEGIN

CHILD_DEFINE(Pair, Object);

void Pair::initRoot() {
    Object::root()->addChild("Pair", root());
}

CHILD_END
