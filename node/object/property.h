#ifndef CHILD_PROPERTY_H
#define CHILD_PROPERTY_H

#include "node/object.h"

CHILD_BEGIN

#define CHILD_PROPERTY(ARGS...) \
new Property(Node::context()->child("Object", "Property"), ##ARGS)

class Property : public Object {
    CHILD_DECLARE(Property, Object, Object);
public:
    explicit Property(Node *origin) : Object(origin) {}

    CHILD_FORK_METHOD(Property);

    virtual Node *run(Node *receiver, Message *message, Primitive *code = NULL) {
        Q_UNUSED(receiver);
        return child("get")->run(this, message, code);
    }
};

CHILD_END

#endif // CHILD_PROPERTY_H
