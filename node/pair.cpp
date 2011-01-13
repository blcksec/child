#include "node/pair.h"

CHILD_BEGIN

CHILD_DEFINE(Pair, Object);

void Pair::initRoot() {
    Object::root()->addChild("Pair", this);
}

CHILD_END
