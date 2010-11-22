#include "child/world.h"

namespace Child {
//    CHILD_IMPLEMENTATION(Application, Object);
    World::World() {
        setOrigin(Object::root());
        addParent("World", this);
    }
}
