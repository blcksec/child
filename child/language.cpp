#include "child/language.h"

CHILD_BEGIN

namespace Language {
    ObjectPointer &root() {
        Q_UNUSED(isInitialized);
        static ObjectPointer _root;
        if(!_root) {
            _root = Object::root()->fork();
            Object::root()->addChild("Language", _root);
        }
        return _root;
    }
}

CHILD_END
