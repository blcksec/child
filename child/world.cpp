#include "world.h"

namespace Child {
    bool World::_isInitialized = false;

    World::World() {
        initialize();
        addModule(Module::root()->child("Object"));
        addParent("World", this);
    }

    void World::initialize() {
        if(!_isInitialized) {
            Module::initialize();
            NativeMethod::initialize();
            Object::initialize();
            Text::initialize();
            _isInitialized = true;
        }
    }
}
