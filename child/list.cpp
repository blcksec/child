#include "child/list.h"

CHILD_BEGIN

CHILD_DEFINITION(List, Object);

bool List::initRoot() {
    Object::root()->addChild("List", root());
    return true;
}

CHILD_END
