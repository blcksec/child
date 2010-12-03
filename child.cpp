#include "child.h"
#include "child/node.h"
#include "child/exception.h"

namespace Child {

void init() {
    Node::pushContext(Node::root());
}

} // namespace Child
