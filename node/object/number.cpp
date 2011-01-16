#include "node/object/number.h"

CHILD_BEGIN

CHILD_DEFINE(Number, Element, Object);

void Number::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Number, add, +);
    CHILD_ADD_NATIVE_METHOD(Number, subtract, -);
    CHILD_ADD_NATIVE_METHOD(Number, multiply, *);
    CHILD_ADD_NATIVE_METHOD(Number, divide, /);
    CHILD_ADD_NATIVE_METHOD(Number, modulo, %);

    CHILD_ADD_NATIVE_METHOD(Number, unary_plus, unary+);
    CHILD_ADD_NATIVE_METHOD(Number, unary_minus, unary-);

    CHILD_ADD_NATIVE_METHOD(Number, prefix_increment, prefix++);
    CHILD_ADD_NATIVE_METHOD(Number, prefix_decrement, prefix--);

    CHILD_ADD_NATIVE_METHOD(Number, equal_to, ==);
    CHILD_ADD_NATIVE_METHOD(Number, compare, <=>);
}

CHILD_END
