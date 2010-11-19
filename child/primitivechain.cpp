#include "child/primitivechain.h"

namespace Child {
    PrimitiveChain *PrimitiveChain::_root = PrimitiveChain::root();

    PrimitiveChain *PrimitiveChain::root() {
        if(!_root) {
            _root = new PrimitiveChain;
            _root->setOrigin(Object::root());
            _root->addParent("PrimitiveChain", Object::root());
        }
        return(_root);
    }
}
