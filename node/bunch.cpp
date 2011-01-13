#include "child/bunch.h"

CHILD_BEGIN

CHILD_DEFINE(Bunch, List);

void Bunch::initRoot() {
    Object::root()->addChild("Bunch", this);
}

CHILD_END
