#include "child/language.h"

CHILD_BEGIN

namespace Language {
    ObjectPtr &root() {
        Q_UNUSED(isInitialized);
        static ObjectPtr _root;
        if(!_root) {
            _root = Object::root()->fork();
            Object::root()->addChild("Language", _root);
        }
        return _root;
    }
}

CHILD_END
