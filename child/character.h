#ifndef CHILD_CHARACTER_H
#define CHILD_CHARACTER_H

#include "child/element.h"

CHILD_BEGIN

CHILD_POINTER_DECLARATION(Character, Element);

#define CHILD_CHARACTER(ARGS...) CharacterPointer(new Character(Node::context()->child("Object", "Character"), ##ARGS))

class Character : public GenericElement<CharacterPointer, QChar> {
    CHILD_DECLARATION(Character, Element);
public:
    Character(const Pointer &origin, const QChar &value = QChar::Null) :
        GenericElement<CharacterPointer, QChar>(origin, value) {}

    static void initRoot() { Object::root()->addChild("Character", root()); }
    virtual Pointer fork() const { return new Character(this, value()); }

    virtual const QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(level);
        return debug ? ("'" + QString(value()) + "'") : value();
    }
};

CHILD_POINTER_DEFINITION(Character, Element);

CHILD_END

#endif // CHILD_CHARACTER_H
