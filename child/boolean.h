#ifndef CHILD_BOOLEAN_H
#define CHILD_BOOLEAN_H

#include "child/element.h"

CHILD_BEGIN

CHILD_POINTER_DECLARATION(Boolean, Element);

#define CHILD_BOOLEAN(ARGS...) BooleanPointer(new Boolean(Node::context()->child("Object", "Boolean"), ##ARGS))

class Boolean : public GenericElement<BooleanPointer, bool> {
    CHILD_DECLARATION(Boolean, Element);
public:
    Boolean(const Pointer &origin, const bool value = false) : GenericElement<BooleanPointer, bool>(origin, value) {}

    static void initRoot() { Object::root()->addChild("Boolean", root()); }
    virtual Pointer fork() const { return new Boolean(this, value()); }

    virtual const QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return value() ? "true" : "false";
    }
};

CHILD_POINTER_DEFINITION(Boolean, Element);

CHILD_END

#endif // CHILD_BOOLEAN_H
