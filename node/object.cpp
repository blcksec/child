#include "node/object.h"
#include "node/object/boolean.h"
#include "node/object/number.h"
#include "node/object/message.h"

CHILD_BEGIN

CHILD_DEFINE(Object, Node, Node);

void Object::initRoot() {
    addExtension(ControlFlow::root());

    CHILD_ADD_NATIVE_METHOD(Object, postfix_increment, postfix++);
    CHILD_ADD_NATIVE_METHOD(Object, postfix_decrement, postfix--);

    CHILD_ADD_NATIVE_METHOD(Object, add_assign, +=);
    CHILD_ADD_NATIVE_METHOD(Object, subtract_assign, -=);
    CHILD_ADD_NATIVE_METHOD(Object, multiply_assign, *=);
    CHILD_ADD_NATIVE_METHOD(Object, divide_assign, /=);
    CHILD_ADD_NATIVE_METHOD(Object, modulo_assign, %=);

    CHILD_ADD_NATIVE_METHOD(Object, less_than, <);
    CHILD_ADD_NATIVE_METHOD(Object, less_than_or_equal_to, <=);
    CHILD_ADD_NATIVE_METHOD(Object, greater_than, >);
    CHILD_ADD_NATIVE_METHOD(Object, greater_than_or_equal_to, >=);
}

CHILD_DEFINE_NATIVE_METHOD(Object, postfix_increment) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    Node *temp = copy();
    CHILD_MESSAGE("prefix++")->run(this);
    return temp;
}

CHILD_DEFINE_NATIVE_METHOD(Object, postfix_decrement) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    Node *temp = copy();
    CHILD_MESSAGE("prefix--")->run(this);
    return temp;
}

CHILD_DEFINE_NATIVE_METHOD(Object, add_assign) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("+", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_DEFINE_NATIVE_METHOD(Object, subtract_assign) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("-", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_DEFINE_NATIVE_METHOD(Object, multiply_assign) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("*", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_DEFINE_NATIVE_METHOD(Object, divide_assign) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("/", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_DEFINE_NATIVE_METHOD(Object, modulo_assign) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(2);
    Node *result = CHILD_MESSAGE("%", message->secondInput())->run(message->runFirstInput());
    return CHILD_MESSAGE("=", message->firstInput(), result)->run();
}

CHILD_DEFINE_NATIVE_METHOD(Object, less_than) {
    CHILD_FIND_LAST_MESSAGE;
    return CHILD_BOOLEAN(Number::cast(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() < 0);
}

CHILD_DEFINE_NATIVE_METHOD(Object, less_than_or_equal_to) {
    CHILD_FIND_LAST_MESSAGE;
    return CHILD_BOOLEAN(Number::cast(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() <= 0);
}

CHILD_DEFINE_NATIVE_METHOD(Object, greater_than) {
    CHILD_FIND_LAST_MESSAGE;
    return CHILD_BOOLEAN(Number::cast(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() > 0);
}

CHILD_DEFINE_NATIVE_METHOD(Object, greater_than_or_equal_to) {
    CHILD_FIND_LAST_MESSAGE;
    return CHILD_BOOLEAN(Number::cast(CHILD_MESSAGE("<=>", message->inputs(false))->run(this))->value() >= 0);
}

CHILD_END
