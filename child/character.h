#ifndef CHILD_CHARACTER_H
#define CHILD_CHARACTER_H

#include "child/element.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Character, Element);

#define CHILD_CHARACTER(ARGS...) CharacterPtr(new Character(Node::findInContext("Object")->child("Character"), ##ARGS))

class Character : public GenericElement<CharacterPtr, QChar> {
    CHILD_DECLARATION(Character, Element);
public:
    Character(const NodePtr &origin, const QChar &value = QChar::Null) : GenericElement<CharacterPtr, QChar>(origin, value) {}

    static void initRoot() { Object::root()->addChild("Character", root()); }
    virtual NodePtr fork() const { return new Character(this, value()); }

    virtual const QString inspect() const { return QString("'%1'").arg(value()); }
};

CHILD_PTR_DEFINITION(Character, Element);

CHILD_END

#endif // CHILD_CHARACTER_H
