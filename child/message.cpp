#include "child/message.h"

CHILD_BEGIN

CHILD_DEFINE(Message, Object);

void Message::initRoot() {
    Object::root()->addChild("Message", this);
}

CHILD_DEFINE(Message::Sending, Node);

void Message::Sending::initRoot() {
    Message::root()->addChild("Sending", this);
}

CHILD_END
