#ifndef CHILD_BOOLEAN_H
#define CHILD_BOOLEAN_H

#include "child/object.h"

namespace Child {
    class Boolean : public Object {
        CHILD_DECLARATION(Boolean);
    public:
        static Boolean *fork(Node *world, bool value) {
            return(Boolean::fork(world)->setValue(value));
        }

        virtual void initFork() { _value = Boolean::as(origin())->_value; }

        bool value() const { return(_value); }
        Boolean *setValue(bool value) { _value = value; return(this); }

        virtual const QString inspect() const { return(value() ? "true" : "false"); }
    private:
        bool _value;
    };
}

#endif // CHILD_BOOLEAN_H
