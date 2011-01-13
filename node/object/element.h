#ifndef CHILD_ELEMENT_H
#define CHILD_ELEMENT_H

#include "node/object.h"

CHILD_BEGIN

inline uint qHash(const double &number) { CHILD_TODO; return number; }

template<class T>
class GenericElement : public Object {
public:
    explicit GenericElement(Node *origin, const T &value = NULL) : Object(origin), _value(value) {}

    T value() const { return _value; }
    virtual void setValue(const T &newValue) { _value = newValue; }

    virtual uint hash() const { return qHash(_value); }
private:
    T _value;
};

#define CHILD_ELEMENT(ARGS...) new Element(context()->child("Object", "Element"), ##ARGS)

class Element : public GenericElement<Node *> {
    CHILD_DECLARE(Element, Object, Object);
public:
    explicit Element(Node *origin, Node *value = NULL) :
        GenericElement<Node *>(origin, value) {}

    CHILD_FORK_METHOD(Element, CHILD_FORK_IF_NOT_NULL(value()));

    virtual void setValue(Node *const &newValue) {
        if(newValue != value()) {
            if(value()) removeAnonymousChild(value());
            GenericElement<Node *>::setValue(newValue);
            if(newValue) addAnonymousChild(newValue);
        }
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        return value() ? value()->toString(debug, level) : Object::toString(debug, level); //"NULL";
    }
};

CHILD_END

#endif // CHILD_ELEMENT_H
