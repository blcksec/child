#ifndef CHILD_PAIR_H
#define CHILD_PAIR_H

#include "child/object.h"

CHILD_BEGIN

template<class C, class T1, class T2>
class GenericPair : public Object {
public:
    explicit GenericPair(const Pointer &origin, const T1 &first = NULL, const T2 &second = NULL) :
        Object(origin), _first(first), _second(second) {}

    T1 first() const { return _first; }
    void setFirst(const T1 &first) { _first = first; }
    T2 second() const { return _second; }
    void setSecond(const T2 &second) { _second = second; }

    // aliases...
    T1 key() const { return _first; }
    void setKey(const T1 &key) { _first = key; }
    T2 value() const { return _second; }
    void setValue(const T2 &value) { _second = value; }
private:
    T1 _first;
    T2 _second;
};

CHILD_POINTER_DECLARE(Pair, Object);

#define CHILD_PAIR(ARGS...) PairPointer(new Pair(Node::context()->child("Object", "Pair"), ##ARGS))

class Pair : public GenericPair<PairPointer, Pointer, Pointer> {
    CHILD_DECLARE(Pair, Object);
public:
    explicit Pair(const Pointer &origin, const Pointer &first = NULL, const Pointer &second = NULL) :
        GenericPair<PairPointer, Pointer, Pointer>(origin, first, second) {}

    static void initRoot() { Object::root()->addChild("Pair", root()); }

    virtual Pointer fork() const {
        return new Pair(this, forkIfNotNull(first()), forkIfNotNull(second()));
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        return QString("%1: %2").arg(first()->toString(debug, level), second()->toString(debug, level));
    }
};

CHILD_POINTER_DEFINE(Pair, Object);

CHILD_END

#endif // CHILD_PAIR_H
