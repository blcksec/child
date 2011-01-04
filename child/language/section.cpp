#include "child/language/section.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Section, List);

    void Section::initRoot() {
        Language::root()->addChild("Section", root());
    }
}

CHILD_END
