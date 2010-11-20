#include "child/list.h"

namespace Child {
    List *List::_root = List::root();

    List *List::root() {
        if(!_root) {
            _root = new List;
            _root->setOrigin(Object::root());
            _root->addParent("List", Object::root());
        }
        return(_root);
    }
}
