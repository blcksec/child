#include "child/object.h"
#include "child/boolean.h"
#include "child/number.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_DEFINE(Object, Node);

CHILD_NATIVE_METHOD_DEFINE(Object, postfix_increment) {
    CHILD_CHECK_INPUT_SIZE(0);
    CHILD_TODO;
}

CHILD_NATIVE_METHOD_DEFINE(Object, postfix_decrement) {
    CHILD_CHECK_INPUT_SIZE(0);
    CHILD_TODO;
}

CHILD_NATIVE_METHOD_DEFINE(Object, add_assign) {
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("+", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_NATIVE_METHOD_DEFINE(Object, subtract_assign) {
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("-", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_NATIVE_METHOD_DEFINE(Object, multiply_assign) {
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("*", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_NATIVE_METHOD_DEFINE(Object, divide_assign) {
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("/", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_NATIVE_METHOD_DEFINE(Object, modulo_assign) {
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("%", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_NATIVE_METHOD_DEFINE(Object, less_than) {
    return CHILD_BOOLEAN(Number::cast(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() < 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, less_than_or_equal_to) {
    return CHILD_BOOLEAN(Number::cast(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() <= 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, greater_than) {
    return CHILD_BOOLEAN(Number::cast(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() > 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, greater_than_or_equal_to) {
    return CHILD_BOOLEAN(Number::cast(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() >= 0);
}

CHILD_NATIVE_METHOD_DEFINE(Object, loop) {
    CHILD_CHECK_INPUT_SIZE(0, 2);
    Node *result = NULL;
    try {
        Node *code;
        if(message->hasInput(0) && (code = message->hasInputOrSection(1, "body"))) { // finite loop
            unsigned int n = message->runInput(0)->toDouble();
            for(unsigned int i = 1; i <= n; i++) result = code->run();
        } else // infinite loop
            while(true) result = message->runInputOrSection(0, "body");
    } catch(const Break &brk) { result = brk.result; }
    return result;
}

Node *Object::whileOrUntil(Message *message, bool isWhile) {
    CHILD_CHECK_INPUT_SIZE(1, 2);
    Node *result = NULL;
    try {
        Node *test;
        Node *code = message->hasInputOrSection(1, "body");
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
    } catch(const Break &brk) { result = brk.result; }
    return result;
}

CHILD_NATIVE_METHOD_DEFINE(Object, break) {
    CHILD_CHECK_INPUT_SIZE(0, 1);
    Node *result = message->hasInput(0) ? message->runFirstInput() : NULL;
    throw Break(result);
}

CHILD_NATIVE_METHOD_DEFINE(Object, return) {
    CHILD_CHECK_INPUT_SIZE(0, 1);
    Node *result = message->hasInput(0) ? message->runFirstInput() : NULL;
    throw Return(result);
}

CHILD_END
