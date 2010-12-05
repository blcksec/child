#ifndef CHILD_NUMBER_H
#define CHILD_NUMBER_H

#include "child/object.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Number, Object);

#define CHILD_NUMBER(ARGS...) NumberPtr(new Number(Node::findInContext("Object")->child("Number"), ##ARGS))

class Number : public Object {
    CHILD_DECLARATION(Number, Object);
public:
    Number(const NodePtr &origin, double value = 0) : Object(origin), _value(value) {}
    static void initRoot() { Object::root()->addChild("Number", root()); }
    virtual NodePtr fork() const { return NodePtr(new Number(NodePtr(this), _value)); }

    double value() const { return _value; }
    void setValue(double value) { _value = value; }

    virtual const QString inspect() const { return QString("%1").arg(value()); }
private:
    double _value;
};

CHILD_PTR_DEFINITION(Number, Object);

CHILD_END

#endif // CHILD_NUMBER_H
