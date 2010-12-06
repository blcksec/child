#ifndef CHILD_BOOLEAN_H
#define CHILD_BOOLEAN_H

#include "child/object.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Boolean, Object);

#define CHILD_BOOLEAN(ARGS...) BooleanPtr(new Boolean(Node::findInContext("Object")->child("Boolean"), ##ARGS))

class Boolean : public Object {
    CHILD_DECLARATION(Boolean, Object);
public:
    Boolean(const NodePtr &origin, bool value = false) : Object(origin), _value(value) {}
    static void initRoot() { Object::root()->addChild("Boolean", root()); }
    virtual NodePtr fork() const { return new Boolean(this, _value); }

    bool value() const { return _value; }
    void setValue(bool value) { _value = value; }

    virtual const QString inspect() const { return value() ? "true" : "false"; }
private:
    bool _value;
};

CHILD_PTR_DEFINITION(Boolean, Object);

CHILD_END

#endif // CHILD_BOOLEAN_H
