#ifndef CHILD_OBJECT_H
#define CHILD_OBJECT_H

#include "child/node.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Object, Node);

#define CHILD_OBJECT(...) ObjectPtr(new Object(__VA_ARGS__))

class Object : public Node {
    CHILD_DECLARATION(Object, Node);
public:
    Object(const NodePtr &origin) : Node(origin) {} // root constructor

    Object(const ObjectPtr &origin) : Node(origin) {} // fork constructor

    Object() : Node(find("Object")) {} // convenience constructor
};

CHILD_PTR_DEFINITION(Object, Node);

CHILD_END

#endif // CHILD_OBJECT_H
