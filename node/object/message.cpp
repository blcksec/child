#include "node/object/boolean.h"
#include "node/object/message.h"

CHILD_BEGIN

CHILD_DEFINE(Message, Object, Object);

void Message::initRoot() {
}

Node *Message::run(Node *receiver) {
    Node *rcvr = isParented() ? receiver->parent() : receiver;
    bool wasFound = true;
    Node *result = rcvr->child(name(), isQuestioned() ? &wasFound : NULL);
    if(!wasFound) Primitive::skip(CHILD_BOOLEAN(false));
    Alias *alias = Alias::dynamicCast(result);
    if(alias && !alias->target().isEmpty()) result = rcvr->child(alias->target());
    if(!isEscaped()) {
        if(result->isAutoRunnable()) {
            CHILD_PUSH_RUN(this);
            result = result->run(rcvr);
        } else if(inputs(false)) {
            Message *forkMessage = fork();
            forkMessage->setName("fork");
            result = forkMessage->run(result);
        }
    }
    return result;
}

QString Message::toString(bool debug, short level) const {
    QString str;
    if(isEscaped()) str += "\\";
    if(isParented()) str += "@";
    str += name();
    if(inputs(false)) str += "(" + inputs()->toString(debug, level) + ")";
    if(isQuestioned()) str += "?";
    if(isExclaimed()) str += "!";
    if(isEllipsed()) str += "...";
    if(hasCodeInput()) str += " " + codeInputName() + "...";
    if(outputs(false)) str += " -> " + outputs()->toString(debug, level);
    return str;
}

CHILD_END
