#ifndef NATIVEMETHOD_H
#define NATIVEMETHOD_H

#include "child/module.h"

#define CMETHODPTR(METH) static_cast<ModuleMethodPtr>(&METH)

namespace Child {
    class NativeMethod : public Module {
    public:
        static NativeMethod *root();

        virtual NativeMethod *fork() const { return(_fork(this)->setMethod(_method)); }
        virtual NativeMethod *fork(ModuleMethodPtr method) { return(_fork(this)->setMethod(method)); }

        ModuleMethodPtr method() const { return(_method); }
        NativeMethod *setMethod(ModuleMethodPtr method) { _method = method; return(this); }
    private:
        static NativeMethod *_root;
        ModuleMethodPtr _method;
    };
}

#endif // NATIVEMETHOD_H
