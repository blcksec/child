#ifndef OBJECT_H
#define OBJECT_H

#include "child/module.h"

namespace Child {
    class Object : public Module {
    public:
        virtual Object *clone() const { return(_clone(this)); }
    };
}

#endif // OBJECT_H
