#include "node/object/number.h"

CHILD_BEGIN

CHILD_DEFINE(Number, Element, Object);

void Number::initRoot() {
    CHILD_NATIVE_METHOD_ADD(Number, add, +);
    CHILD_NATIVE_METHOD_ADD(Number, subtract, -);
    CHILD_NATIVE_METHOD_ADD(Number, multiply, *);
    CHILD_NATIVE_METHOD_ADD(Number, divide, /);
    CHILD_NATIVE_METHOD_ADD(Number, modulo, %);

    CHILD_NATIVE_METHOD_ADD(Number, unary_plus, unary+);
    CHILD_NATIVE_METHOD_ADD(Number, unary_minus, unary-);

    CHILD_NATIVE_METHOD_ADD(Number, prefix_increment, prefix++);
    CHILD_NATIVE_METHOD_ADD(Number, prefix_decrement, prefix--);

    CHILD_NATIVE_METHOD_ADD(Number, equal_to, ==);
    CHILD_NATIVE_METHOD_ADD(Number, compare, <=>);
}

CHILD_END
