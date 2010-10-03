#include "child/object.h"

namespace Child {
    Object *Object::_root = Object::root();

    Object *Object::root() {
        if(!_root) {
            _root = new Object;
            _root->setOrigin(Node::root());
            _root->addParent("Object", Node::root());
        }
        return(_root);
    }
}
