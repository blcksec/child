#include "child/number.h"

namespace Child {
    Number *Number::_root = Number::root();

    Number *Number::root() {
        if(!_root) {
            _root = new Number;
            _root->setOrigin(Object::root());
            _root->addParent("Number", Object::root());
        }
        return(_root);
    }
}
