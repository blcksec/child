#include "child/section.h"

namespace Child {
    Section *Section::_root = Section::root();

    Section *Section::root() {
        if(!_root) {
            _root = new Section;
            _root->setOrigin(Object::root());
            _root->addParent("Section", Object::root());
        }
        return(_root);
    }
}
