#include "node/object/character.h"

CHILD_BEGIN

CHILD_DEFINE(Character, Element, Object);

void Character::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Character, equal_to, ==);
    CHILD_ADD_NATIVE_METHOD(Character, compare, <=>);
}

CHILD_END
