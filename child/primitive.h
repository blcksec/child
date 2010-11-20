#ifndef CHILD_PRIMITIVE_H
#define CHILD_PRIMITIVE_H

#include "child/object.h"

#define CHILD_PRIMITIVE(EXPRESSION) static_cast<Primitive *>(EXPRESSION)

namespace Child {
    class Primitive : public Object {
    public:
        static Primitive *root();
        static Primitive *fork(Node *world) { return(CHILD_PRIMITIVE(world->child("Primitive"))->fork()); }

        virtual Primitive *fork() { notYetImplemented(); return(NULL); }
    private:
        static Primitive *_root;
    };
}

#endif // CHILD_PRIMITIVE_H
