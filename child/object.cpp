#include "child/object.h"

CHILD_BEGIN

CHILD_DEFINITION(Object, Node);

bool Object::initRoot() {
    Node::root()->addChild("Object", root());
    return true;
}

CHILD_END
