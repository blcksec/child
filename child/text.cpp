#include "child/text.h"

namespace Child {
    void Text::initialize() {
        if(!Module::root()->child("Object")->hasDirectChild("Text")) {
            Text *txt = new Text;
            txt->addModule(Module::root()->child("Object"));
            txt->addParent("Text", Module::root()->child("Object"));
        }
    }
//    Text *const Text::create(Module *const &world, const QString &value) {
//        Text *txt = new Text(world->fatalSend("Text"));
//        txt->setValue(value);
//        return(txt);
//    }

//    Module *Text::toText() {
//        return(this);
//    }
}
