#ifndef CHILD_PAIR_H
#define CHILD_PAIR_H

#include "child/object.h"

CHILD_BEGIN

template<class P, class T1, class T2>
class GenericPair : public Object {
public:
    GenericPair(const NodePtr &origin, T1 first = NULL, T2 second = NULL) :
        Object(origin), _first(first), _second(second) {}

    virtual NodePtr fork() const { return new GenericPair(this, _first->fork(), _second->fork()); }

    T1 first() const { return _first; }
    void setFirst(T1 first) { _first = first; }
    T2 second() const { return _second; }
    void setSecond(T2 second) { _second = second; }

    // aliases...
    T1 key() const { return _first; }
    void setKey(T1 key) { _first = key; }
    T2 value() const { return _second; }
    void setValue(T2 value) { _second = value; }

    virtual const QString toString(bool debug = false) const {
        return QString("%1: %2").arg(first()->toString(debug), second()->toString(debug));
    }
private:
    T1 _first;
    T2 _second;
};

CHILD_PTR_DECLARATION(Pair, Object);

#define CHILD_PAIR(ARGS...) PairPtr(new Pair(Node::findInContext("Object")->child("Pair"), ##ARGS))

class Pair : public GenericPair<PairPtr, NodePtr, NodePtr> {
    CHILD_DECLARATION(Pair, Object);
public:
    Pair(const NodePtr &origin, NodePtr first = NULL, NodePtr second = NULL) :
        GenericPair<PairPtr, NodePtr, NodePtr>(origin, first, second) {}

    static void initRoot() { Object::root()->addChild("Pair", root()); }
    virtual NodePtr fork() const { return new Pair(this, first()->fork(), second()->fork()); }
};

CHILD_PTR_DEFINITION(Pair, Object);

CHILD_END

#endif // CHILD_PAIR_H
