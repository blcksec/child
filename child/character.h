#ifndef CHILD_CHARACTER_H
#define CHILD_CHARACTER_H

#include "child/element.h"
#include "child/boolean.h"
#include "child/number.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_POINTER_DECLARE(Character, Element);

#define CHILD_CHARACTER(ARGS...) CharacterPointer(new Character(Node::context()->child("Object", "Character"), ##ARGS))

class Character : public GenericElement<CharacterPointer, QChar> {
    CHILD_DECLARE(Character, Element);
public:
    explicit Character(const Pointer &origin, const QChar &value = QChar::Null) :
        GenericElement<CharacterPointer, QChar>(origin, value) {}

    static void initRoot() {
        Object::root()->addChild("Character", root());
        CHILD_NATIVE_METHOD_ADD(Character, equal_to, ==);
        CHILD_NATIVE_METHOD_ADD(Character, compare, <=>);
    }

    virtual Pointer fork() const { return new Character(this, value()); }

    virtual bool isEqualTo(const Pointer &other) const {
        return value() == CharacterPointer(other)->value();
    }

    CHILD_NATIVE_METHOD_DECLARE(equal_to) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_BOOLEAN(value() == message->runFirstInput()->toChar());
    }

    virtual short compare(const Pointer &other) const {
        return compare(CharacterPointer(other)->value());
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

CHILD_POINTER_DEFINE(Character, Element);

CHILD_END

#endif // CHILD_CHARACTER_H
