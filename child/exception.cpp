#include "child.h"
#include "child/exception.h"

namespace Child {

ExceptionPtr Exception::_root = Exception::root();

ExceptionPtr &Exception::root() {
    if(!_root) {
        _root = new Exception;
        _root->setOrigin(Node::root());
        Node::root()->setChild("Exception", _root);
        Exception::initRoot();
    }
    return _root;
}

void Exception::initRoot() {}

ExceptionPtr Exception::make() {
    ExceptionPtr f(new Exception);
    f->setOrigin(context()->child("Exception"));
    return f;
}

} // namespace Child
