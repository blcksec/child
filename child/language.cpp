#include "child/language.h"

CHILD_BEGIN

namespace Language {
    Object *root() {
        Q_UNUSED(isInitialized);
        static Object *_root = NULL;
        if(!_root) {
            _root = Object::root()->fork();
            Object::root()->addChild("Language", _root);
        }
        return _root;
    }
}

CHILD_END
