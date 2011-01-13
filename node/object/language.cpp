#include "node/object/language.h"

CHILD_BEGIN

namespace Language {
    Object *root() {
        static Object *_root = NULL;
        if(!_root) {
            _root = Object::root()->fork();
            Object::root()->addChild("Language", _root);
            _root->declare("Language");
        }
        return _root;
    }
}

CHILD_END
