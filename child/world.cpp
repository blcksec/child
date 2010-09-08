#include "world.h"

namespace Child {
    World::World() {
        addModule(Object::root());
        addParent("World", this);
    }
}
