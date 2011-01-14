#include "node/alias.h"
#include "node/object/message.h"

CHILD_BEGIN

CHILD_DEFINE(Alias, Node, Node);

void Alias::initRoot() {
    CHILD_NATIVE_METHOD_ADD(Alias, init);
}

CHILD_NATIVE_METHOD_DEFINE(Alias, init) {
    CHILD_CHECK_INPUT_SIZE(1);
    Message *msg = Message::dynamicCast(message->firstInput()->value()->value());
    if(!msg) CHILD_THROW(ArgumentException, "expected 'Message'");
    setTarget(msg->name());
    return this;
}

CHILD_END
