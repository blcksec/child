#include "node/nativemethod.h"
#include "node/object/message.h"

CHILD_BEGIN

CHILD_DEFINE(NativeMethod, Node, Node);

void NativeMethod::initRoot() {
}

Node *NativeMethod::run(Node *receiver, Message *message, Primitive *code) {
    if(!code && hasCodeInput())
        return new Message::Sending(Message::Sending::root(), message, receiver);
    else
        return (receiver->*method())(message, code);
}

CHILD_END
