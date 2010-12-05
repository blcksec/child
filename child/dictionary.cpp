#include "child/dictionary.h"

CHILD_BEGIN

CHILD_DEFINITION(Dictionary, Object);

bool Dictionary::initRoot() {
    Object::root()->addChild("Dictionary", root());
    return true;
}

CHILD_END
