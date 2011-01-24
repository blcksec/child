#ifndef CHILD_CHARACTER_H
#define CHILD_CHARACTER_H

#include "node/object/element.h"
#include "node/object/boolean.h"
#include "node/object/number.h"
#include "node/object/message.h"

CHILD_BEGIN

#define CHILD_CHARACTER(ARGS...) new Character(context()->child("Object", "Character"), ##ARGS)

class Character : public GenericElement<QChar> {
    CHILD_DECLARE(Character, Element, Object);
public:
    explicit Character(Node *origin, const QChar &value = QChar::Null) :
        GenericElement<QChar>(origin, QChar::Null) { setValue(value); }

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Character);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Character, value());

    virtual bool isEqualTo(const Node *other) const {
        return value() == cast(other)->value();
    }

    CHILD_DECLARE_NATIVE_METHOD(equal_to) {
        CHILD_FIND_LAST_MESSAGE;
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

    CHILD_DECLARE_NATIVE_METHOD(compare) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(compare(message->runFirstInput()->toChar()));
    }

    virtual double toDouble(bool *okPtr = NULL) const {
        Q_UNUSED(okPtr);
        return value().unicode();
    };

    virtual QChar toChar() const { return value(); };

    virtual QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(level);
        return debug ? ("'" + QString(value()) + "'") : value();
    }
};

CHILD_END

#endif // CHILD_CHARACTER_H
