#include "node/language/sourcecodedictionary.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(SourceCodeDictionary, Dictionary);

    void SourceCodeDictionary::initRoot() {
        Language::root()->addChild("SourceCodeDictionary", this);
    }
}

CHILD_END
