#ifndef CHILD_OBJECT_H
#define CHILD_OBJECT_H

#include "child/node.h"

namespace Child {

CHILD_PTR_DECLARATION(Object, Node);

class Object : public Node {
    CHILD_DECLARATION(Object, Node);
public:
    Object(const NodePtr &origin) : Node(origin) {} // root constructor

    Object(const ObjectPtr &origin) : Node(origin) {} // fork constructor

    Object() : Node(find("Object")) {} // convenience constructor
};

CHILD_PTR_DEFINITION(Object, Node);

} // namespace Child

#endif // CHILD_OBJECT_H
