#include "node/alias.h"
#include "node/object/message.h"

CHILD_BEGIN

CHILD_DEFINE(Alias, Node, Node);

void Alias::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Alias, init);
}

CHILD_DEFINE_NATIVE_METHOD(Alias, init) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    Message *msg = Message::dynamicCast(message->firstInput()->value()->value());
    if(!msg) CHILD_THROW(ArgumentException, "expected 'Message'");
    setTarget(msg->name());
    return this;
}

CHILD_END
