#ifndef CHILD_BOOLEAN_H
#define CHILD_BOOLEAN_H

#include "child/element.h"
#include "child/message.h"

CHILD_BEGIN

#define CHILD_BOOLEAN(ARGS...) new Boolean(Node::context()->child("Object", "Boolean"), ##ARGS)

class Boolean : public GenericElement<bool> {
    CHILD_DECLARE(Boolean, Element);
public:
    explicit Boolean(Node *origin, const bool value = false) : GenericElement<bool>(origin, value) {}

    static void initRoot() {
        Object::root()->addChild("Boolean", root());
        CHILD_NATIVE_METHOD_ADD(Boolean, equal_to, ==);
    }

    CHILD_FORK_METHOD(Boolean, value());

    virtual bool isEqualTo(const Node *other) const {
        return value() == Boolean::cast(other)->value();
    }

    CHILD_NATIVE_METHOD_DECLARE(equal_to) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_BOOLEAN(value() == message->runFirstInput()->toBool());
    }

    virtual bool toBool() const { return value(); };

    virtual double toDouble() const { return value() ? 1 : 0; };

    virtual QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return value() ? "true" : "false";
    }
};

CHILD_END

#endif // CHILD_BOOLEAN_H
