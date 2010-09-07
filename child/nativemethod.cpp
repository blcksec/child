#include "child/nativemethod.h"

namespace Child {
    void NativeMethod::initialize() {
        if(!Module::root()->hasDirectChild("NativeMethod")) {
            NativeMethod *mod = new NativeMethod;
            mod->addModule(Module::root());
            mod->addParent("NativeMethod", Module::root());
        }
    }

//    NativeMethod *const NativeMethod::create(Module *const &world, ModuleMethodPtr const &method) {
//        Module *n = world->fatalSend("NativeMethod");
//        NativeMethod *meth = new NativeMethod(n);
//        meth->setMethod(method);
//        return(meth);
//    }
}
