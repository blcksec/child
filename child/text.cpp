#include "child/text.h"

namespace Child {
    Text *Text::_root = Text::root();

    Text *Text::root() {
        if(!_root) {
            _root = new Text;
            _root->addModule(Object::root());
            _root->addParent("Text", Object::root());

            NativeMethod *meth = NativeMethod::root()->fork(CMETHODPTR(Text::upcase));
            _root->addDirectChild("upcase", meth);
        }
        return(_root);
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
