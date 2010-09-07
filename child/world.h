#ifndef WORLD_H
#define WORLD_H

#include "child/object.h"

namespace Child {
    class World : public Object {
    public:
        World();
        virtual Object *clone() const { return(_clone(this)); }
    };
}

#endif // WORLD_H
