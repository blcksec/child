#include "child/nativemethod.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_DEFINE(NativeMethod, Node);

void NativeMethod::initRoot() {
    Node::root()->addChild("NativeMethod", this);
}

Node *NativeMethod::run(Node *receiver, Message *message, Primitive *code) {
    if(!code && hasCodeInput())
        return new Message::Sending(Message::Sending::root(), message, receiver);
    else
        return (receiver->*method())(message, code);
}

CHILD_END
