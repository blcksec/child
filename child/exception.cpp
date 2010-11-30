#include "child.h"
#include "child/exception.h"

namespace Child {

CHILD_EXCEPTION_DEFINITION(Exception, Node, Node);

const QString Exception::report() const {
    QString str;
    if(!_file.isEmpty()) str += QString("%1:").arg(QFileInfo(_file).fileName());
    if(_line != 0) str += QString("%1: ").arg(_line);
    str += className();
    if(!_message.isEmpty()) str += QString(": %1").arg(_message);
    if(!_function.isEmpty()) str += QString(" in '%1'").arg(_function);
    return str;
}

CHILD_EXCEPTION_DEFINITION(LexerException, Exception, Node);

} // namespace Child
