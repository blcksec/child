#include "child/text.h"

namespace Child {
//    CHILD_IMPLEMENTATION(Text, Object);
    Text *Text::_root = Text::root();

    Text *Text::root() {
        if(!_root) {
            _root = new Text;
            _root->setOrigin(Object::root());
            _root->addParent("Text", Object::root());

            NativeMethod *meth = NativeMethod::fork(_root, CHILD_METHODPTR(Text::upcase));
            _root->addDirectChild("upcase", meth);
        }
        return(_root);
    }
}
