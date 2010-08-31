#include "child/text.h"

namespace Child {
    Text *Text::clone() const {
        Text *mod = new Text;
        mod->addModule(const_cast<Text *>(this));
        return(mod);
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
