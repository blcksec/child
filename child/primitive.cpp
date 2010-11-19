#include "child/primitive.h"

namespace Child {
    Primitive *Primitive::_root = Primitive::root();

    Primitive *Primitive::root() {
        if(!_root) {
            _root = new Primitive;
            _root->setOrigin(Object::root());
            _root->addParent("Primitive", Object::root());
        }
        return(_root);
    }
}
