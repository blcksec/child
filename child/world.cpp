#include "child/world.h"

namespace Child {
    World::World() {
        setBaseModule(Object::root());
        addParent("World", this);
    }
}
