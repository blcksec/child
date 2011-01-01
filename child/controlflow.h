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

        CHILD_NATIVE_METHOD_ADD(ControlFlow, loop);

        CHILD_NATIVE_METHOD_ADD(ControlFlow, break);
    }

    CHILD_FORK_METHOD(ControlFlow);

    CHILD_NATIVE_METHOD_DECLARE(if) { return If::make(this, message, true); }
    CHILD_NATIVE_METHOD_DECLARE(unless) { return If::make(this, message, false); }

    CHILD_NATIVE_METHOD_DECLARE(loop) { return Loop::make(this, message); }

    CHILD_NATIVE_METHOD_DECLARE(break);

    // === If ===

    #define CHILD_IF(ARGS...) new If(Node::context()->child("ControlFlow", "If"), ##ARGS)

    class If : public Node {
        CHILD_DECLARE(If, Node);
    public:
        explicit If(Node *origin, Node *receiver = NULL, Node *test = NULL, bool isIf = true) :
            Node(origin), _receiver(receiver), _test(test), _isIf(isIf) {}

        static void initRoot() {
            ControlFlow::root()->addChild("If", root());
        }

        CHILD_FORK_METHOD(If, CHILD_FORK_IF_NOT_NULL(_receiver), CHILD_FORK_IF_NOT_NULL(_test), _isIf);

        static Node *make(Node *receiver, Message *message, bool isIf);
        virtual Node *receive(Language::Primitive *primitive);
    private:
        Node *_receiver;
        Node *_test;
        bool _isIf; // false for "unless" mode
    };

    // === Loop ===

    #define CHILD_LOOP(ARGS...) new Loop(Node::context()->child("ControlFlow", "Loop"), ##ARGS)

    class Loop : public Node {
        CHILD_DECLARE(Loop, Node);
    public:
        explicit Loop(Node *origin, Node *receiver = NULL, HugeInteger count = -1) :
            Node(origin), _receiver(receiver), _count(count) {}

        static void initRoot() {
            ControlFlow::root()->addChild("Loop", root());
        }

        CHILD_FORK_METHOD(Loop, CHILD_FORK_IF_NOT_NULL(_receiver), _count);

        static Node *make(Node *receiver, Message *message);
        virtual Node *receive(Language::Primitive *primitive);
    private:
        Node *_receiver;
        HugeInteger _count; // < 0 for infinite loop
    };

    // === Break ===

    class Break {
    public:
        Node *result;
        Break(Node *result = NULL) : result(result) {}
    };
};

CHILD_END

#endif // CHILD_CONTROL_FLOW_H
