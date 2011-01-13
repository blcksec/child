#include "node/object.h"
#include "node/boolean.h"
#include "node/number.h"
#include "node/message.h"

CHILD_BEGIN

CHILD_DEFINE(Object, Node);

void Object::initRoot() {
    Node::root()->addChild("Object", this);

    addExtension(ControlFlow::root());

    CHILD_NATIVE_METHOD_ADD(Object, postfix_increment, postfix++);
    CHILD_NATIVE_METHOD_ADD(Object, postfix_decrement, postfix--);

    CHILD_NATIVE_METHOD_ADD(Object, add_assign, +=);
    CHILD_NATIVE_METHOD_ADD(Object, subtract_assign, -=);
    CHILD_NATIVE_METHOD_ADD(Object, multiply_assign, *=);
    CHILD_NATIVE_METHOD_ADD(Object, divide_assign, /=);
    CHILD_NATIVE_METHOD_ADD(Object, modulo_assign, %=);

    CHILD_NATIVE_METHOD_ADD(Object, less_than, <);
    CHILD_NATIVE_METHOD_ADD(Object, less_than_or_equal_to, <=);
    CHILD_NATIVE_METHOD_ADD(Object, greater_than, >);
    CHILD_NATIVE_METHOD_ADD(Object, greater_than_or_equal_to, >=);

    CHILD_NATIVE_METHOD_ADD(Object, return);
}

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

CHILD_NATIVE_METHOD_DEFINE(Object, return) {
    CHILD_CHECK_INPUT_SIZE(0, 1);
    Node *result = message->hasInput(0) ? message->runFirstInput() : NULL;
    throw Return(result);
}

CHILD_END
