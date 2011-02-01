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

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(ControlFlow);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(ControlFlow);

    CHILD_DECLARE_NATIVE_METHOD(if) { return ifOrUnless(true); }
    CHILD_DECLARE_NATIVE_METHOD(unless) { return ifOrUnless(false); }
private:
    Node *ifOrUnless(bool isIf);
public:
    enum LoopType { Loop, While, Until };
    CHILD_DECLARE_NATIVE_METHOD(loop) { return loop(Loop); }
    CHILD_DECLARE_NATIVE_METHOD(while) { return loop(While); }
    CHILD_DECLARE_NATIVE_METHOD(until) { return loop(Until); }
private:
    Node *loop(LoopType type);
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
};

CHILD_END

#endif // CHILD_CONTROL_FLOW_H
