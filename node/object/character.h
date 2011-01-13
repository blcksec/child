#ifndef CHILD_CHARACTER_H
#define CHILD_CHARACTER_H

#include "node/element.h"
#include "node/boolean.h"
#include "node/number.h"
#include "node/message.h"

CHILD_BEGIN

#define CHILD_CHARACTER(ARGS...) new Character(Node::context()->child("Object", "Character"), ##ARGS)

class Character : public GenericElement<QChar> {
    CHILD_DECLARE(Character, Element);
public:
    explicit Character(Node *origin, const QChar &value = QChar::Null) :
        GenericElement<QChar>(origin, value) {}

    CHILD_FORK_METHOD(Character, value());

    virtual bool isEqualTo(const Node *other) const {
        return value() == cast(other)->value();
    }

    CHILD_NATIVE_METHOD_DECLARE(equal_to) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_BOOLEAN(value() == message->runFirstInput()->toChar());
    }

    virtual short compare(const Node *other) const {
        return compare(cast(other)->value());
    }

    short compare(const QChar &other) const {
        if(value() > other) return 1;
        else if(value() < other) return -1;
        else return 0;
    }

    CHILD_NATIVE_METHOD_DECLARE(compare) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(compare(message->runFirstInput()->toChar()));
    }

    virtual double toDouble() const { return value().unicode(); };

    virtual QChar toChar() const { return value(); };

    virtual QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(level);
        return debug ? ("'" + QString(value()) + "'") : value();
    }
};

CHILD_END

#endif // CHILD_CHARACTER_H
