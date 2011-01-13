#include "node/object/message.h"

CHILD_BEGIN

CHILD_DEFINE(Message, Object, Object);

void Message::initRoot() {
}

CHILD_DEFINE(Message::Sending, Node, Message);

void Message::Sending::initRoot() {
}

CHILD_END
