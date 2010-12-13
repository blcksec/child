#ifndef CHILD_NUMBER_H
#define CHILD_NUMBER_H

#include "child/element.h"

CHILD_BEGIN

CHILD_POINTER_DECLARATION(Number, Element);

#define CHILD_NUMBER(ARGS...) NumberPointer(new Number(Node::context()->child("Object", "Number"), ##ARGS))

class Number : public GenericElement<NumberPointer, double> {
    CHILD_DECLARATION(Number, Element);
public:
    Number(const Pointer &origin, const double value = 0) : GenericElement<NumberPointer, double>(origin, value) {}

    static void initRoot() { Object::root()->addChild("Number", root()); }
    virtual Pointer fork() const { return new Number(this, value()); }

    virtual const QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return QString("%1").arg(value());
    }
};

CHILD_POINTER_DEFINITION(Number, Element);

CHILD_END

#endif // CHILD_NUMBER_H
