#include "child/message.h"

namespace Child {
    Message *Message::_root = Message::root();

    Message *Message::root() {
        if(!_root) {
            _root = new Message;
            _root->setOrigin(Object::root());
            _root->addParent("Message", Object::root());
        }
        return(_root);
    }
}
