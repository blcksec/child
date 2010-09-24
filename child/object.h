#ifndef OBJECT_H
#define OBJECT_H

#include "child/module.h"

namespace Child {
    class Object : public Module {
    public:
        static Object *root();
        virtual Object *fork() { return(_fork(this)); }
    private:
        static Object *_root;
    };
}

#endif // OBJECT_H
