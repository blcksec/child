#ifndef CHILD_NUMBER_H
#define CHILD_NUMBER_H

#include <math.h>

#include "child/element.h"
#include "child/language/argument.h"

CHILD_BEGIN

CHILD_POINTER_DECLARE(Number, Element);

#define CHILD_NUMBER(ARGS...) NumberPointer(new Number(Node::context()->child("Object", "Number"), ##ARGS))

class Number : public GenericElement<NumberPointer, double> {
    CHILD_DECLARE(Number, Element);
public:
    Number(const Pointer &origin, const double value = 0) : GenericElement<NumberPointer, double>(origin, value) {}

    static void initRoot() {
        Object::root()->addChild("Number", root());
        CHILD_NATIVE_METHOD_ADD(Number, add, +);
        CHILD_NATIVE_METHOD_ADD(Number, subtract, -);
        CHILD_NATIVE_METHOD_ADD(Number, multiply, *);
        CHILD_NATIVE_METHOD_ADD(Number, divide, /);
        CHILD_NATIVE_METHOD_ADD(Number, modulo, %);
        CHILD_NATIVE_METHOD_ADD(Number, unary_plus);
        CHILD_NATIVE_METHOD_ADD(Number, unary_minus);
    }

    virtual Pointer fork() const { return new Number(this, value()); }

    CHILD_NATIVE_METHOD_DECLARE(add) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() + inputs->first()->run()->toDouble());
    }

    CHILD_NATIVE_METHOD_DECLARE(subtract) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() - inputs->first()->run()->toDouble());
    }

    CHILD_NATIVE_METHOD_DECLARE(multiply) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() * inputs->first()->run()->toDouble());
    }

    CHILD_NATIVE_METHOD_DECLARE(divide) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() / inputs->first()->run()->toDouble());
    }

    CHILD_NATIVE_METHOD_DECLARE(modulo) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(llround(value()) % llround(inputs->first()->run()->toDouble()));
    }

    CHILD_NATIVE_METHOD_DECLARE(unary_plus) {
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_NUMBER(value());
    }

    CHILD_NATIVE_METHOD_DECLARE(unary_minus) {
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_NUMBER(-value());
    }

    virtual const double toDouble() const { return value(); };

    virtual const QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return QString("%1").arg(value());
    }
};

CHILD_POINTER_DEFINE(Number, Element);

CHILD_END

#endif // CHILD_NUMBER_H
