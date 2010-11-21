#include "child/sourcecode.h"

namespace Child {
    SourceCode *SourceCode::_root = SourceCode::root();

    SourceCode *SourceCode::root() {
        if(!_root) {
            _root = new SourceCode;
            _root->setOrigin(Object::root());
            _root->addParent("SourceCode", Object::root());
        }
        return(_root);
    }
}
