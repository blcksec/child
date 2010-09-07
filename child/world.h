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
        static void initialize();
        virtual Object *fork() const { return(_fork(this)); }
    private:
        static bool _isInitialized;
    };
}

#endif // WORLD_H
