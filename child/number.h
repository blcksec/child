#ifndef CHILD_NUMBER_H
#define CHILD_NUMBER_H

#include <math.h>

#include "child/element.h"
#include "child/language/argument.h"

CHILD_BEGIN

CHILD_POINTER_DECLARATION(Number, Element);

#define CHILD_NUMBER(ARGS...) NumberPointer(new Number(Node::context()->child("Object", "Number"), ##ARGS))

class Number : public GenericElement<NumberPointer, double> {
    CHILD_DECLARATION(Number, Element);
public:
    Number(const Pointer &origin, const double value = 0) : GenericElement<NumberPointer, double>(origin, value) {}

    static void initRoot() {
        Object::root()->addChild("Number", root());
        CHILD_ADD_NATIVE_METHOD(Number, add); root()->aliasChild("add", "+");
        CHILD_ADD_NATIVE_METHOD(Number, subtract); root()->aliasChild("subtract", "-");
        CHILD_ADD_NATIVE_METHOD(Number, multiply); root()->aliasChild("multiply", "*");
        CHILD_ADD_NATIVE_METHOD(Number, divide); root()->aliasChild("divide", "/");
        CHILD_ADD_NATIVE_METHOD(Number, modulo); root()->aliasChild("modulo", "%");
        CHILD_ADD_NATIVE_METHOD(Number, unary_plus);
        CHILD_ADD_NATIVE_METHOD(Number, unary_minus);
    }
    virtual Pointer fork() const { return new Number(this, value()); }

    NumberPointer add(const NumberPointer &other) const { return CHILD_NUMBER(value() + other->value()); }
    CHILD_NATIVE_METHOD_DECLARE(add) { return add(inputs->first()->run()); }

    NumberPointer subtract(const NumberPointer &other) const { return CHILD_NUMBER(value() - other->value()); }
    CHILD_NATIVE_METHOD_DECLARE(subtract) { return subtract(inputs->first()->run()); }

    NumberPointer multiply(const NumberPointer &other) const { return CHILD_NUMBER(value() * other->value()); }
    CHILD_NATIVE_METHOD_DECLARE(multiply) { return multiply(inputs->first()->run()); }

    NumberPointer divide(const NumberPointer &other) const { return CHILD_NUMBER(value() / other->value()); }
    CHILD_NATIVE_METHOD_DECLARE(divide) { return divide(inputs->first()->run()); }

    NumberPointer modulo(const NumberPointer &other) const { return CHILD_NUMBER(llround(value()) % llround(other->value())); }
    CHILD_NATIVE_METHOD_DECLARE(modulo) { return modulo(inputs->first()->run()); }

    NumberPointer unary_plus() const { return CHILD_NUMBER(value()); }
    CHILD_NATIVE_METHOD_DECLARE(unary_plus) { Q_UNUSED(inputs); return unary_plus(); }

    NumberPointer unary_minus() const { return CHILD_NUMBER(-value()); }
    CHILD_NATIVE_METHOD_DECLARE(unary_minus) { Q_UNUSED(inputs); return unary_minus(); }

    virtual const QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return QString("%1").arg(value());
    }
};

CHILD_POINTER_DEFINITION(Number, Element);

CHILD_END

#endif // CHILD_NUMBER_H
