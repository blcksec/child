#ifndef CHILD_CONTROL_FLOW_H
#define CHILD_CONTROL_FLOW_H

#include "child/node.h"
#include "child/exception.h"
#include "child/nativemethod.h"

CHILD_BEGIN

#define CHILD_CONTROL_FLOW(ARGS...) new ControlFlow(Node::context()->child("ControlFlow"), ##ARGS)

class ControlFlow : public Node {
    CHILD_DECLARE(ControlFlow, Node);
public:
    explicit ControlFlow(Node *origin) : Node(origin) {}

    static void initRoot() {
        Node::root()->addChild("ControlFlow", root());

        CHILD_NATIVE_METHOD_ADD(ControlFlow, if);
        CHILD_NATIVE_METHOD_ADD(ControlFlow, unless);

        CHILD_NATIVE_METHOD_WITH_CODE_INPUT_ADD(ControlFlow, loop);

        CHILD_NATIVE_METHOD_WITH_CODE_INPUT_ADD(ControlFlow, while);
        CHILD_NATIVE_METHOD_WITH_CODE_INPUT_ADD(ControlFlow, until);

        CHILD_NATIVE_METHOD_ADD(ControlFlow, break);
    }

    CHILD_FORK_METHOD(ControlFlow);

    CHILD_NATIVE_METHOD_WITH_CODE_INPUT_DECLARE(if) { return ifOrUnless(message, code, true); }
    CHILD_NATIVE_METHOD_WITH_CODE_INPUT_DECLARE(unless) { return ifOrUnless(message, code, false); }
private:
    Node *ifOrUnless(Message *message, Primitive *code, bool isIf);
public:

    CHILD_NATIVE_METHOD_WITH_CODE_INPUT_DECLARE(loop);

    CHILD_NATIVE_METHOD_WITH_CODE_INPUT_DECLARE(while) { return whileOrUntil(message, code, true); }
    CHILD_NATIVE_METHOD_WITH_CODE_INPUT_DECLARE(until) { return whileOrUntil(message, code, false); }
private:
    Node *whileOrUntil(Message *message, Primitive *code, bool isWhile);
public:

    CHILD_NATIVE_METHOD_DECLARE(break);

    // === Break ===

    class Break {
    public:
        Node *result;
        Break(Node *result = NULL) : result(result) {}
    };
};

CHILD_END

#endif // CHILD_CONTROL_FLOW_H
