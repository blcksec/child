#include "child/list.h"

CHILD_BEGIN

CHILD_DEFINE(List, Object);

void List::initRoot() {
    Object::root()->addChild("List", root());
}

CHILD_END
