#include "child/object.h"

namespace Child {
    void Object::initialize() {
        if(!Module::root()->hasDirectChild("Object")) {
            Object *mod = new Object;
            mod->addModule(Module::root());
            mod->addParent("Object", Module::root());
        }
    }
}
