#include "child/block.h"

namespace Child {
    Block *Block::_root = Block::root();

    Block *Block::root() {
        if(!_root) {
            _root = new Block;
            _root->setOrigin(Object::root());
            _root->addParent("Block", Object::root());
        }
        return(_root);
    }
}
