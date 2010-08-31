#ifndef NATIVEMETHOD_H
#define NATIVEMETHOD_H

#include "child/module.h"

#define CHILD_METHOD(METH) static_cast<Module::MethodPtr>(&METH)

namespace Child {
//    class NativeMethod : public Module {
//    public:
//        NativeMethod(Module *const &genre = NULL, Module *const &parent = NULL) : Module(genre, parent), _method(NULL) {}

//        static NativeMethod *const create(Module *const &world, ModuleMethodPtr const &method);

//        ModuleMethodPtr const method() const { return(_method); }
//        void setMethod(ModuleMethodPtr const &method) { _method = method; }
//    private:
//        ModuleMethodPtr _method;
//    };
}

#endif // NATIVEMETHOD_H
