#ifndef CHILD_NUMBER_H
#define CHILD_NUMBER_H

#include "child/object.h"

namespace Child {
    class Number : public Object {
        CHILD_DECLARATION(Number, Object, Object);
    public:
        static Number *fork(Node *world, double value) {
            return Number::fork(world)->setValue(value);
        }

        virtual void initFork() { setValue(Number::as(origin())->_value); }

        double value() const { return _value; }
        Number *setValue(double value) { _value = value; return this; }

        virtual const QString inspect() const { return QString("%1").arg(value()); }
    private:
        double _value;
    };
}

#endif // CHILD_NUMBER_H
