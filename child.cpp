#include "child.h"
#include "child/node.h"
#include "child/exception.h"

namespace Child {

void init() {
    Node::initRoot();
    Exception::initRoot();
    LexerException::initRoot();
    ParserException::initRoot();
    RuntimeException::initRoot();
    ArgumentException::initRoot();
    NullPointerException::initRoot();
    IndexOutOfBoundsException::initRoot();
    NotFoundException::initRoot();
    DuplicateException::initRoot();
    OperatingSystemException::initRoot();
    FileSystemException::initRoot();
    Node::pushContext(Node::root());
}

} // namespace Child
