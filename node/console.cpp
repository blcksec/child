#include "node/console.h"
#include "node/object/message.h"
#include "node/object/text.h"

CHILD_BEGIN

CHILD_DEFINE(Console, Node, Node);

void Console::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Console, write);
    CHILD_ADD_NATIVE_METHOD(Console, print);
    CHILD_ADD_NATIVE_METHOD(Console, alert);

    CHILD_ADD_NATIVE_METHOD(Console, read);
}

CHILD_DEFINE_NATIVE_METHOD(Console, write) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    QString str = message->runFirstInput()->toString();
    QTextStream(stdout) << str.toUtf8();
    return CHILD_TEXT(str);
}

CHILD_DEFINE_NATIVE_METHOD(Console, print) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    QString str = message->runFirstInput()->toString();
    CHILD_MESSAGE("write", CHILD_ARGUMENT_BUNCH(CHILD_TEXT(str + "\n")))->run(this);
    return CHILD_TEXT(str);
}

CHILD_DEFINE_NATIVE_METHOD(Console, alert) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    QString str = message->runFirstInput()->toString();
    QTextStream(stderr) << (str + "\n").toUtf8();
    return CHILD_TEXT(str);
}

CHILD_DEFINE_NATIVE_METHOD(Console, read) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0, 1);
    if(message->hasAnInput())
        CHILD_MESSAGE("write", message->inputs())->run(this);
    QString str;
    QTextStream(stdin) >> str;
    return CHILD_TEXT(str);
}

CHILD_END
