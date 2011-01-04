#include "child/language/sourcecode.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(SourceCode, Object);

    void SourceCode::initRoot() {
        Language::root()->addChild("SourceCode", root());
    }

    CHILD_DEFINE(SourceCodeDictionary, Dictionary);

    void SourceCodeDictionary::initRoot() {
        Language::root()->addChild("SourceCodeDictionary", root());
    }
}

CHILD_END
