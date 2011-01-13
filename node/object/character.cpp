#include "node/object/character.h"

CHILD_BEGIN

CHILD_DEFINE(Character, Element, Object);

void Character::initRoot() {
    CHILD_NATIVE_METHOD_ADD(Character, equal_to, ==);
    CHILD_NATIVE_METHOD_ADD(Character, compare, <=>);
}

CHILD_END
