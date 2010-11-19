#ifndef CHILD_OBJECT_H
#define CHILD_OBJECT_H

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

#endif // CHILD_OBJECT_H
