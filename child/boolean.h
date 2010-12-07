#ifndef CHILD_BOOLEAN_H
#define CHILD_BOOLEAN_H

#include "child/element.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Boolean, Element);

#define CHILD_BOOLEAN(ARGS...) BooleanPtr(new Boolean(Node::findInContext("Object")->child("Boolean"), ##ARGS))

class Boolean : public GenericElement<BooleanPtr, bool> {
    CHILD_DECLARATION(Boolean, Element);
public:
    Boolean(const NodePtr &origin, const bool value = false) : GenericElement<BooleanPtr, bool>(origin, value) {}

    static void initRoot() { Object::root()->addChild("Boolean", root()); }
    virtual NodePtr fork() const { return new Boolean(this, value()); }

    virtual const QString toString(bool debug = false) const {
        #pragma unused(debug)
        return value() ? "true" : "false";
    }
};

CHILD_PTR_DEFINITION(Boolean, Element);

CHILD_END

#endif // CHILD_BOOLEAN_H
