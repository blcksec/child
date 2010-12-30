#ifndef CHILD_CONTROL_FLOW_H
#define CHILD_CONTROL_FLOW_H

#include "child/node.h"
#include "child/exception.h"

CHILD_BEGIN

#define CHILD_CONTROL_FLOW(ARGS...) new ControlFlow(Node::context()->child("ControlFlow"), ##ARGS)

class ControlFlow : public Node {
    CHILD_DECLARE(ControlFlow, Node);
public:
    explicit ControlFlow(Node *origin) : Node(origin) {}

    static void initRoot() {
        Node::root()->addChild("ControlFlow", root());
    }

    CHILD_FORK_METHOD(ControlFlow);

    #define CHILD_IF(ARGS...) new If(Node::context()->child("ControlFlow", "If"), ##ARGS)

    class If : public Node {
        CHILD_DECLARE(If, Node);
    public:
        explicit If(Node *origin) : Node(origin) {}

        static void initRoot() {
            ControlFlow::root()->addChild("If", root());
            ControlFlow::root()->addChild("if", root());
        }

        CHILD_FORK_METHOD(If);
    };
};

CHILD_END

#endif // CHILD_CONTROL_FLOW_H
