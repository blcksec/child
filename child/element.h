#ifndef CHILD_ELEMENT_H
#define CHILD_ELEMENT_H

#include "child/object.h"

CHILD_BEGIN

inline uint qHash(const double &number) { CHILD_TODO; return number; }

template<class P, class T>
class GenericElement : public Object {
public:
    GenericElement(const Pointer &origin, const T &value = NULL) : Object(origin), _value(value) {}

    T value() const { return _value; }
    void setValue(const T &value) { _value = value; }

    virtual uint hash() const { return qHash(_value); }
private:
    T _value;
};

CHILD_POINTER_DECLARATION(Element, Object);

#define CHILD_ELEMENT(ARGS...) ElementPointer(new Element(Node::context()->child("Object", "Element"), ##ARGS))

class Element : public GenericElement<ElementPointer, Pointer> {
    CHILD_DECLARATION(Element, Object);
public:
    Element(const Pointer &origin, const Pointer &value = NULL) : GenericElement<ElementPointer, Pointer>(origin, value) {}
    static void initRoot() { Object::root()->addChild("Element", root()); }
    virtual Pointer fork() const { return new Element(this, forkIfNotNull(value())); }
    virtual const QString toString(bool debug = false, short level = 0) const {
        return value() ? value()->toString(debug, level) : "NULL";
    }
};

CHILD_POINTER_DEFINITION(Element, Object);

CHILD_END

#endif // CHILD_ELEMENT_H
