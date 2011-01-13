#include "node/application.h"

CHILD_BEGIN

CHILD_DEFINE(Application, Object);

void Application::initRoot() {
    Object::root()->addChild("Application", this);
}

CHILD_END
