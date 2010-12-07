#ifndef CHILD_NUMBER_H
#define CHILD_NUMBER_H

#include "child/element.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Number, Element);

#define CHILD_NUMBER(ARGS...) NumberPtr(new Number(Node::findInContext("Object")->child("Number"), ##ARGS))

class Number : public GenericElement<NumberPtr, double> {
    CHILD_DECLARATION(Number, Element);
public:
    Number(const NodePtr &origin, const double value = 0) : GenericElement<NumberPtr, double>(origin, value) {}

    static void initRoot() { Object::root()->addChild("Number", root()); }
    virtual NodePtr fork() const { return new Number(this, value()); }

    virtual const QString toString(bool debug = false) const {
        #pragma unused(debug)
        return QString("%1").arg(value());
    }
};

CHILD_PTR_DEFINITION(Number, Element);

CHILD_END

#endif // CHILD_NUMBER_H
