#include "child/object.h"

namespace Child {
    Object *Object::_root = Object::root();

    Object *Object::root() {
        if(!_root) {
            _root = new Object;
            _root->addModule(Module::root());
            _root->addParent("Object", Module::root());
        }
        return(_root);
    }
}
