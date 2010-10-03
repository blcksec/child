#ifndef OBJECT_H
#define OBJECT_H

#include "child/node.h"

namespace Child {
    class Object : public Node {
    public:
        static Object *root();
        virtual Object *fork() { return(_fork(this)); }
    private:
        static Object *_root;
    };
}

#endif // OBJECT_H
