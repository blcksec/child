#ifndef CHILD_NUMBER_H
#define CHILD_NUMBER_H

#include <math.h>

#include "child/element.h"
#include "child/boolean.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_POINTER_DECLARE(Number, Element);

#define CHILD_NUMBER(ARGS...) NumberPointer(new Number(Node::context()->child("Object", "Number"), ##ARGS))

class Number : public GenericElement<NumberPointer, double> {
    CHILD_DECLARE(Number, Element);
public:
    explicit Number(const Pointer &origin, const double value = 0) : GenericElement<NumberPointer, double>(origin, value) {}

    static void initRoot() {
        Object::root()->addChild("Number", root());
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

    virtual Pointer fork() const { return new Number(this, value()); }

    CHILD_NATIVE_METHOD_DECLARE(add) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() + message->runFirstInput()->toDouble());
    }

    CHILD_NATIVE_METHOD_DECLARE(subtract) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() - message->runFirstInput()->toDouble());
    }

    CHILD_NATIVE_METHOD_DECLARE(multiply) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() * message->runFirstInput()->toDouble());
    }

    CHILD_NATIVE_METHOD_DECLARE(divide) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(value() / message->runFirstInput()->toDouble());
    }

    CHILD_NATIVE_METHOD_DECLARE(modulo) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(llround(value()) % llround(message->runFirstInput()->toDouble()));
    }

    CHILD_NATIVE_METHOD_DECLARE(unary_plus) {
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_NUMBER(value());
    }

    CHILD_NATIVE_METHOD_DECLARE(unary_minus) {
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_NUMBER(-value());
    }

    CHILD_NATIVE_METHOD_DECLARE(prefix_increment) {
        CHILD_CHECK_INPUT_SIZE(0);
        setValue(value() + 1);
        return this;
    }

    CHILD_NATIVE_METHOD_DECLARE(prefix_decrement) {
        CHILD_CHECK_INPUT_SIZE(0);
        setValue(value() - 1);
        return this;
    }

    virtual bool isEqualTo(const Pointer &other) const {
        return value() == NumberPointer(other)->value();
    }

    CHILD_NATIVE_METHOD_DECLARE(equal_to) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_BOOLEAN(value() == message->runFirstInput()->toDouble());
    }

    virtual short compare(const Pointer &other) const {
        return compare(NumberPointer(other)->value());
    }

    short compare(const double &other) const {
        if(value() > other) return 1;
        else if(value() < other) return -1;
        else return 0;
    }

    CHILD_NATIVE_METHOD_DECLARE(compare) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(compare(message->runFirstInput()->toDouble()));
    }

    virtual double toDouble() const { return value(); };

    virtual QChar toChar() const { return QChar(int(value())); };

    virtual QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return QString("%1").arg(value());
    }
};

CHILD_POINTER_DEFINE(Number, Element);

CHILD_END

#endif // CHILD_NUMBER_H
