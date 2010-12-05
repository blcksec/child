#ifndef CHILD_OBJECT_H
#define CHILD_OBJECT_H

#include "child/node.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Object, Node);

#define CHILD_OBJECT(ARGS...) ObjectPtr(new Object(Node::findInContext("Object"), ##ARGS))

class Object : public Node {
    CHILD_DECLARATION(Object, Node);
public:
    Object(const NodePtr &origin) : Node(origin) {}
    static void initRoot() { Node::root()->addChild("Object", root()); }
    virtual NodePtr fork() const { return NodePtr(new Object(NodePtr(this))); }
};

CHILD_PTR_DEFINITION(Object, Node);

CHILD_END

#endif // CHILD_OBJECT_H
