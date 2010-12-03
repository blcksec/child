#include "child/object.h"

namespace Child {

CHILD_DEFINITION(Object, Node);

bool Object::initRoot() {
    Node::root()->addChild("Object", root());
    return true;
}

}  // namespace Child
