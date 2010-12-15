#ifndef CHILD_BOOLEAN_H
#define CHILD_BOOLEAN_H

#include "child/element.h"
#include "child/language/argument.h"

CHILD_BEGIN

CHILD_POINTER_DECLARE(Boolean, Element);

#define CHILD_BOOLEAN(ARGS...) BooleanPointer(new Boolean(Node::context()->child("Object", "Boolean"), ##ARGS))

class Boolean : public GenericElement<BooleanPointer, bool> {
    CHILD_DECLARE(Boolean, Element);
public:
    Boolean(const Pointer &origin, const bool value = false) : GenericElement<BooleanPointer, bool>(origin, value) {}

    static void initRoot() {
        Object::root()->addChild("Boolean", root());
    }

    virtual Pointer fork() const { return new Boolean(this, value()); }

    virtual const bool toBool() const { return value(); };

    virtual const double toDouble() const { return value() ? 1 : 0; };

    virtual const QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return value() ? "true" : "false";
    }
};

CHILD_POINTER_DEFINE(Boolean, Element);

CHILD_END

#endif // CHILD_BOOLEAN_H
