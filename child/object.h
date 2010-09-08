#ifndef OBJECT_H
#define OBJECT_H

#include "child/module.h"

namespace Child {
    class Object : public Module {
    public:
        static Object *root();
        virtual Object *fork() const { return(_fork(this)); }
    private:
        static Object *_root;
    };
}

#endif // OBJECT_H
