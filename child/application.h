#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "child/object.h"

#define CHILD_APPLICATION(EXPRESSION) static_cast<Application *>(EXPRESSION)

namespace Child {
    class Application : public Object {
    public:
        static Application *root();
    private:
        static Application *_root;
    };
}

#endif // CHILD_APPLICATION_H
