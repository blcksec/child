#ifndef CHILD_ELEMENT_H
#define CHILD_ELEMENT_H

#include "node/object.h"

CHILD_BEGIN

inline uint qHash(const double &number) { CHILD_TODO; return number; }

template<typename T>
class GenericElement : public Object {
public:
    explicit GenericElement(Node *origin, const T &defaultValue = T()) : Object(origin), _value(defaultValue) {}

    T value() const { return _value; }

    virtual void setValue(const T &newValue) {
        if(newValue != value()) {
            _value = newValue;
            hasChanged();
        }
    }

    virtual void hasChanged() {}

    virtual uint hash() const { return qHash(_value); }
private:
    T _value;
};

template<class T>
class GenericNodeElement : public GenericElement<T *> {
public:
    using GenericElement<T *>::value;

    explicit GenericNodeElement(Node *origin) : GenericElement<T *>(origin, NULL) {}

    virtual void setValue(T *newValue) {
        if(newValue != value()) {
            if(value()) removeAnonymousChild(value());
            GenericElement<T *>::setValue(newValue);
            if(newValue) addAnonymousChild(newValue);
        }
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        return value() ? value()->toString(debug, level) : Object::toString(debug, level);
    }
};

#define CHILD_ELEMENT(ARGS...) new Element(context()->child("Object", "Element"), ##ARGS)

class Element : public GenericNodeElement<Node> {
    CHILD_DECLARE(Element, Object, Object);
public:
    explicit Element(Node *origin, Node *value = NULL) : GenericNodeElement<Node>(origin) { setValue(value); }

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Element);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Element, CHILD_FORK_IF_NOT_NULL(value()));
};

CHILD_END

#endif // CHILD_ELEMENT_H
