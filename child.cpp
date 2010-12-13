#include "child.h"
#include "child/node.h"

CHILD_BEGIN

void init() {
    Node::pushContext(Node::root());
}

CHILD_END
