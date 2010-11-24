#include "child/language.h"

namespace Child {
    namespace Language {
        Object *root() {
            static Object *_root;
            if(!_root) {
                _root = new Object;
                _root->setOrigin(Object::root());
                _root->addParent("Language", Object::root());
            }
            return _root;
        }
    }
}
