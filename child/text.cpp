#include "child/text.h"

namespace Child {
    Text *Text::_root = Text::root();

    Text *Text::root() {
        if(!_root) {
            _root = new Text;
            _root->setOrigin(Object::root());
            _root->addParent("Text", Object::root());

            NativeMethod *meth = NativeMethod::root()->fork(CHILD_METHODPTR(Text::upcase));
            _root->addDirectChild("upcase", meth);
        }
        return(_root);
    }
}
