#include "child/object.h"
#include "child/boolean.h"
#include "child/number.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_DEFINE(Object, Node);

CHILD_NATIVE_METHOD_DEFINE(Object, less_than) {
    return CHILD_BOOLEAN(NumberPointer(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() < 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, less_than_or_equal_to) {
    return CHILD_BOOLEAN(NumberPointer(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() <= 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, greater_than) {
    return CHILD_BOOLEAN(NumberPointer(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() > 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, greater_than_or_equal_to) {
    return CHILD_BOOLEAN(NumberPointer(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() >= 0);
}

Pointer Object::ifOrUnless(const MessagePointer &message, bool isIf) {
    CHILD_CHECK_INPUT_SIZE(1, 3);
    Pointer result = message->runFirstInput();
    if(result->toBool() == isIf)
        return message->runInputOrSection(1, "body", result);
    else
        return message->runInputOrSection(2, "else", result);
}

CHILD_NATIVE_METHOD_DEFINE(Object, loop) {
    CHILD_CHECK_INPUT_SIZE(0, 2);
    Pointer result;
    try {
        Pointer code;
        if(message->hasInput(0) && (code = message->hasInputOrSection(1, "body"))) { // finite loop
            unsigned int n = message->runInput(0)->toDouble();
            for(unsigned int i = 1; i <= n; i++) result = code->run();
        } else // infinite loop
            while(true) result = message->runInputOrSection(0, "body");
    } catch(Break brk) { result = brk.result; }
    return result;
}

Pointer Object::whileOrUntil(const MessagePointer &message, bool isWhile) {
    CHILD_CHECK_INPUT_SIZE(1, 2);
    Pointer result;
    Pointer test;
    Pointer code = message->hasInputOrSection(1, "body");
    while(true) {
        if(isWhile) {
            test = message->runFirstInput();
            if(test->toBool()) result = test; else break;
        }
        if(code) result = code->run();
        if(!isWhile) {
            test = message->runFirstInput();
            if(test->toBool()) break;
        }
    }
    if(!code) result = test;
    return result;
}

CHILD_NATIVE_METHOD_DEFINE(Object, break) {
    CHILD_CHECK_INPUT_SIZE(0, 1);
    Pointer result = message->hasInput(0) ? message->runFirstInput() : Pointer::null();
    throw Break(result);
}

CHILD_END
