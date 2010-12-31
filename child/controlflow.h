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

        CHILD_NATIVE_METHOD_ADD(ControlFlow, unless);
    }

    CHILD_FORK_METHOD(ControlFlow);

    CHILD_NATIVE_METHOD_DECLARE(unless) { return If::ifOrUnless(this, message, false); }

    // === If ===

    #define CHILD_IF(ARGS...) new If(Node::context()->child("ControlFlow", "If"), ##ARGS)

    class If : public Node {
        CHILD_DECLARE(If, Node);
    public:
        explicit If(Node *origin, Node *receiver = NULL, Node *test = NULL, bool isIf = true) :
            Node(origin), _receiver(receiver), _test(test), _isIf(isIf) {}

        static void initRoot() {
            ControlFlow::root()->addChild("If", root());
            ControlFlow::root()->addChild("if", root());
        }

        CHILD_FORK_METHOD(If, CHILD_FORK_IF_NOT_NULL(_receiver), CHILD_FORK_IF_NOT_NULL(_test), _isIf);

        virtual Node *receive(Language::Primitive *primitive);

        virtual Node *run(Node *receiver, Message *message) {
            return _test ? this : ifOrUnless(receiver, message, true);
        }

        static Node *ifOrUnless(Node *receiver, Message *message, bool isIf);
    private:
        Node *_receiver;
        Node *_test;
        bool _isIf; // false for "unless" mode
    };

    // === Skip ===

    class Skip {
    public:
        Node *result;
        Skip(Node *result = NULL) : result(result) {}
    };
};

CHILD_END

#endif // CHILD_CONTROL_FLOW_H
