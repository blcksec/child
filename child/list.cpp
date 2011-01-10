#include "child/list.h"

CHILD_BEGIN

CHILD_DEFINE(List, Object);

void List::initRoot() {
    Object::root()->addChild("List", this);
}

CHILD_END
