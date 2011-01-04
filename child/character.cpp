#include "child/character.h"

CHILD_BEGIN

CHILD_DEFINE(Character, Element);

void Character::initRoot() {
    Object::root()->addChild("Character", root());
    CHILD_NATIVE_METHOD_ADD(Character, equal_to, ==);
    CHILD_NATIVE_METHOD_ADD(Character, compare, <=>);
}

CHILD_END
