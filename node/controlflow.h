#ifndef CHILD_CONTROL_FLOW_H
#define CHILD_CONTROL_FLOW_H

#include "node.h"
#include "node/exception.h"
#include "node/nativemethod.h"

CHILD_BEGIN

#define CHILD_CONTROL_FLOW(ARGS...) new ControlFlow(context()->child("ControlFlow"), ##ARGS)

class ControlFlow : public Node {
    CHILD_DECLARE(ControlFlow, Node, Node);
public:
    explicit ControlFlow(Node *origin) : Node(origin) {}

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(ControlFlow);

    CHILD_DECLARE_NATIVE_METHOD(if) { return ifOrUnless(true); }
    CHILD_DECLARE_NATIVE_METHOD(unless) { return ifOrUnless(false); }
private:
    Node *ifOrUnless(bool isIf);
public:

    CHILD_DECLARE_NATIVE_METHOD(loop);

    CHILD_DECLARE_NATIVE_METHOD(while) { return whileOrUntil(true); }
    CHILD_DECLARE_NATIVE_METHOD(until) { return whileOrUntil(false); }
private:
    Node *whileOrUntil(bool isWhile);
public:
    // === Break ===

    class Break {
    public:
        Break(Node *result = NULL) : result(result) {}

        Node *result;
    };

    CHILD_DECLARE_NATIVE_METHOD(break);

    // === Continue ===

    class Continue {
    public:
        Continue() {}
    };

    CHILD_DECLARE_NATIVE_METHOD(continue);

    // === Exception handling ===

    CHILD_DECLARE_NATIVE_METHOD(throw);
};

CHILD_END

#endif // CHILD_CONTROL_FLOW_H
