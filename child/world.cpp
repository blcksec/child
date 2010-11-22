#include "child/world.h"

namespace Child {
    World::World() {
        setOrigin(Object::root());
        addParent("World", this);
    }
}
