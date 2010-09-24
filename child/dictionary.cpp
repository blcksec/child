#include "dictionary.h"

namespace Child {
    Dictionary *Dictionary::_root = Dictionary::root();

    Dictionary *Dictionary::root() {
        if(!_root) {
            _root = new Dictionary;
            _root->setBaseModule(Object::root());
            _root->addParent("Dictionary", Object::root());
        }
        return(_root);
    }
}
