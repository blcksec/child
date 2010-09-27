#ifndef WORLD_H
#define WORLD_H

#include "child/object.h"

namespace Child {
    class World : public Object {
    public:
        World();
        virtual World *fork() { return(_fork(this)); }
    };
}

#endif // WORLD_H
