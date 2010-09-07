#ifndef OBJECT_H
#define OBJECT_H

#include "child/module.h"

namespace Child {
    class Object : public Module {
    public:
        static void initialize();
        virtual Object *fork() const { return(_fork(this)); }
    };
}

#endif // OBJECT_H
