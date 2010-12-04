#include "child/list.h"

namespace Child {

CHILD_DEFINITION(List, Object);

bool List::initRoot() {
    Object::root()->addChild("List", root());
    return true;
}

} // namespace Child
