#ifndef CHILD_CONSOLE_H
#define CHILD_CONSOLE_H

#include "node.h"

CHILD_BEGIN

#define CHILD_CONSOLE(ARGS...) new Console(context()->child("Console"), ##ARGS)

class Console : public Node {
    CHILD_DECLARE(Console, Node, Node);
public:
    explicit Console(Node *origin) : Node(origin) {}

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Console);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Console);

    CHILD_DECLARE_NATIVE_METHOD(write);
    CHILD_DECLARE_NATIVE_METHOD(print);
    CHILD_DECLARE_NATIVE_METHOD(alert);
    CHILD_DECLARE_NATIVE_METHOD(read);
};

CHILD_END

#endif // CHILD_CONSOLE_H
