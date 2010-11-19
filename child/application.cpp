#include "child/application.h"

namespace Child {
    Application *Application::_root = Application::root();

    Application *Application::root() {
        if(!_root) {
            _root = new Application;
            _root->setOrigin(Object::root());
            _root->addParent("Application", Object::root());
        }
        return(_root);
    }
}
