#ifndef CHILD_WORLD_H
#define CHILD_WORLD_H

#include "child/object.h"

namespace Child {
    class World : public Object {
    public:
        World();
        virtual World *fork() { return(_fork(this)); }
    };
}

#endif // CHILD_WORLD_H
