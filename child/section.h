#ifndef CHILD_SECTION_H
#define CHILD_SECTION_H

#include "child/object.h"

#define CHILD_SECTION(EXPRESSION) static_cast<Section *>(EXPRESSION)

namespace Child {
    class Section : public Object {
    public:
        static Section *root();
        static Section *fork(Node *world) { return(CHILD_SECTION(world->child("Section"))->fork()); }

        virtual Section *fork() { notYetImplemented(); return(NULL); }
    private:
        static Section *_root;
    };
}

#endif // CHILD_SECTION_H
