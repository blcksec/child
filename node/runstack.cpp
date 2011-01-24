#include "node/runstack.h"

CHILD_BEGIN

CHILD_DEFINE(RunStack, Node, Node);

void RunStack::initRoot() {
}

void RunStack::dump() const { foreach(Node *run, _stack) run->dump(); }

CHILD_END
