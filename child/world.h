#ifndef CHILD_WORLD_H
#define CHILD_WORLD_H

#include "child/object.h"

#define CHILD_WORLD(EXPRESSION) static_cast<World *>(EXPRESSION)

namespace Child {
    class World : public Object {
    public:
        static World *fork(Node *world) { return(CHILD_WORLD(world->child("World"))->fork()); }

        World();
        virtual World *fork() { return(_fork(this)); }
    };
}

#endif // CHILD_WORLD_H
