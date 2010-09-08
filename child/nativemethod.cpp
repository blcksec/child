#include "child/nativemethod.h"

namespace Child {
    NativeMethod *NativeMethod::_root = NativeMethod::root();

    NativeMethod *NativeMethod::root() {
        if(!_root) {
            _root = new NativeMethod;
            _root->addModule(Module::root());
            _root->addParent("NativeMethod", Module::root());
        }
        return(_root);
    }
}
