#ifndef CHILD_PRIMITIVE_H
#define CHILD_PRIMITIVE_H

#include "child/object.h"

#define CHILD_PRIMITIVE(EXPRESSION) static_cast<Primitive *>(EXPRESSION)

namespace Child {
    class Primitive : public Object {
    public:
        static Primitive *root();
    private:
        static Primitive *_root;
    };
}

#endif // CHILD_PRIMITIVE_H
