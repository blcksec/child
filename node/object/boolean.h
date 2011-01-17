#ifndef CHILD_BOOLEAN_H
#define CHILD_BOOLEAN_H

#include "node/object/element.h"
#include "node/object/message.h"

CHILD_BEGIN

#define CHILD_BOOLEAN(ARGS...) new Boolean(context()->child("Object", "Boolean"), ##ARGS)

class Boolean : public GenericElement<bool> {
    CHILD_DECLARE(Boolean, Element, Object);
public:
    explicit Boolean(Node *origin, bool value = false) : GenericElement<bool>(origin, false) { setValue(value); }

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Boolean, value());

    virtual bool isEqualTo(const Node *other) const {
        return value() == Boolean::cast(other)->value();
    }

    CHILD_DECLARE_NATIVE_METHOD(equal_to) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_BOOLEAN(value() == message->runFirstInput()->toBool());
    }

    virtual bool toBool() const { return value(); };

    virtual double toDouble() const { return value() ? 1 : 0; };

    virtual QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(debug);
        Q_UNUSED(level);
        return value() ? "yes" : "no";
    }
};

CHILD_END

#endif // CHILD_BOOLEAN_H
