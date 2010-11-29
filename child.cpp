#include "child.h"

namespace Child {

void init() {
    pushContext(Node::root());
}

const NodePtr context() { return _contextStack.top(); }
void pushContext(const NodePtr &node) { _contextStack.push(node); }
const NodePtr popContext() { return _contextStack.pop(); }

} // namespace Child
