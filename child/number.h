#ifndef CHILD_NUMBER_H
#define CHILD_NUMBER_H

#include "child/object.h"

#define CHILD_NUMBER(EXPRESSION) static_cast<Number *>(EXPRESSION)

namespace Child {
    class Number : public Object {
    public:
        static Number *root();
        static Number *fork(Node *world) { return(CHILD_NUMBER(world->child("Number"))->fork()); }
        static Number *fork(Node *world, double value) { return(CHILD_NUMBER(world->child("Number"))->fork(value)); }

        virtual Number *fork() { return(_fork(this)->setValue(_value)); }
        virtual Number *fork(double value) { return(_fork(this)->setValue(value)); }

        double value() const { return(_value); }
        Number *setValue(double value) { _value = value; return(this); }

        virtual const QString inspect() const { return(QString("%1").arg(value())); }
    private:
        static Number *_root;
        double _value;
    };
}

#endif // CHILD_NUMBER_H
