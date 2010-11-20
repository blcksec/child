#include "child/operator.h"

namespace Child {
    Operator *Operator::_root = Operator::root();

    Operator *Operator::root() {
        if(!_root) {
            _root = new Operator;
            _root->setOrigin(Object::root());
            _root->addParent("Operator", Object::root());
        }
        return(_root);
    }
}
