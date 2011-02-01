#include "node/nativemethod.h"
#include "node/object/message.h"

CHILD_BEGIN

CHILD_DEFINE(NativeMethod, Node, Node);

void NativeMethod::initRoot() {
}

Node *NativeMethod::run(Node *receiver) {
    CHILD_PUSH_RUN(this);
    return (receiver->*method())();
}

CHILD_END
