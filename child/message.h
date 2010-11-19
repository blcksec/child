#ifndef CHILD_MESSAGE_H
#define CHILD_MESSAGE_H

#include "child/object.h"

#define CHILD_MESSAGE(EXPRESSION) static_cast<Message *>(EXPRESSION)

namespace Child {
    class Message : public Object {
    public:
        static Message *root();
    private:
        static Message *_root;
    };
}

#endif // CHILD_MESSAGE_H
