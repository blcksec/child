#include "child.h"
#include "child/node.h"

namespace Child {

void init() {
    Node::pushContext(Node::root());
}

} // namespace Child
