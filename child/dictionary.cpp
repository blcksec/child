#include "child/dictionary.h"

CHILD_BEGIN

CHILD_DEFINE(Dictionary, Object);

void Dictionary::initRoot() {
    Object::root()->addChild("Dictionary", root());
}

CHILD_END
