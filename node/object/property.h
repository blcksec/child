#ifndef CHILD_PROPERTY_H
#define CHILD_PROPERTY_H

#include "node/object.h"

CHILD_BEGIN

#define CHILD_PROPERTY(ARGS...) \
new Property(context()->child("Object", "Property"), ##ARGS)

class Property : public Object {
    CHILD_DECLARE(Property, Object, Object);
public:
    explicit Property(Node *origin) : Object(origin) { setIsAutoRunnable(true); }

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Property);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Property);

    virtual Node *run(Node *receiver = context()) {
        Q_UNUSED(receiver);
        CHILD_PUSH_RUN(this);
        return child("get")->run(this);
    }
};

CHILD_END

#endif // CHILD_PROPERTY_H
