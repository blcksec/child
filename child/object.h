#ifndef CHILD_OBJECT_H
#define CHILD_OBJECT_H

#include "child/node.h"

#define CHILD_OBJECT(EXPRESSION) static_cast<Object *>(EXPRESSION)

namespace Child {
    class Object : public Node {
    public:
        static Object *root();
        static Object *fork(Node *world) { return(CHILD_OBJECT(world->child("Object"))->fork()); }

        virtual Object *fork() { return(_fork(this)); }
    private:
        static Object *_root;
    };
}

#endif // CHILD_OBJECT_H
