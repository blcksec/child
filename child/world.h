#ifndef WORLD_H
#define WORLD_H

#include "child/module.h"
#include "child/nativemethod.h"
#include "child/object.h"
#include "child/text.h"

namespace Child {
    class World : public Object {
    public:
        World();
        virtual World *fork() { return(_fork(this)); }
    };
}

#endif // WORLD_H
