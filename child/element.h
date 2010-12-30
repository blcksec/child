#ifndef CHILD_ELEMENT_H
#define CHILD_ELEMENT_H

#include "child/object.h"

CHILD_BEGIN

inline uint qHash(const double &number) { CHILD_TODO; return number; }

template<class T>
class GenericElement : public Object {
public:
    explicit GenericElement(Node *origin, const T &value = NULL) : Object(origin), _value(value) {}

    T value() const { return _value; }
    void setValue(const T &value) { _value = value; }

    virtual uint hash() const { return qHash(_value); }
private:
    T _value;
};

#define CHILD_ELEMENT(ARGS...) new Element(Node::context()->child("Object", "Element"), ##ARGS)

class Element : public GenericElement<Node *> {
    CHILD_DECLARE(Element, Object);
public:
    explicit Element(Node *origin, Node *value = NULL) :
        GenericElement<Node *>(origin, value) {}
    static void initRoot() { Object::root()->addChild("Element", root()); }
    CHILD_FORK_METHOD(Element, CHILD_FORK_IF_NOT_NULL(value()));
    virtual QString toString(bool debug = false, short level = 0) const {
        return value() ? value()->toString(debug, level) : "NULL";
    }
};

CHILD_END

#endif // CHILD_ELEMENT_H
