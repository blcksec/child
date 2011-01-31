#include "node/object/boolean.h"
#include "node/object/message.h"
#include "node/object/text.h"

CHILD_BEGIN

CHILD_DEFINE(Message, Object, Object);

void Message::initRoot() {
}

const QString Message::interpoledName() const {
    return interpolableSlices() ? Text::interpolate(name(), interpolableSlices()) : name();
}

Node *Message::run(Node *receiver) {
    Node *rcvr = isParented() ? receiver->parent() : receiver;
    Node *parent;
    Node *result = rcvr->child(interpoledName(), NULL, &parent);
    Alias *alias = Alias::dynamicCast(result);
    if(alias && alias->isNotEmpty())
        result = rcvr->child(alias->names(), NULL, &parent);
    if(!isEscaped()) {
        if(result->isAutoRunnable()) {
            CHILD_PUSH_RUN(this);
            result = result->run(parent);
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
