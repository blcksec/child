#include "child/nativemethod.h"

namespace Child {
    NativeMethod *NativeMethod::_root = NativeMethod::root();

    NativeMethod *NativeMethod::root() {
        if(!_root) {
            _root = new NativeMethod;
            _root->setOrigin(Node::root());
            _root->addParent("NativeMethod", Node::root());
        }
        return(_root);
    }
}
