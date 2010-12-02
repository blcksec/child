#include <QtCore/QFileInfo>

#include "child/exception.h"

namespace Child {

CHILD_EXCEPTION_DEFINITION(Exception, Node, Node);

const QString Exception::report() const {
    QString str;
    if(!file.isEmpty()) str += QString("%1:").arg(QFileInfo(file).fileName());
    if(line != 0) str += QString("%1: ").arg(line);
    str += className();
    if(!message.isEmpty()) str += QString(": %1").arg(message);
    if(!function.isEmpty()) str += QString(" in '%1'").arg(function);
    return str;
}

CHILD_EXCEPTION_DEFINITION(LexerException, Exception, Node);
CHILD_EXCEPTION_DEFINITION(ParserException, Exception, Node);
CHILD_EXCEPTION_DEFINITION(RuntimeException, Exception, Node);
CHILD_EXCEPTION_DEFINITION(ArgumentException, RuntimeException, Node);
CHILD_EXCEPTION_DEFINITION(NullPointerException, RuntimeException, Node);
CHILD_EXCEPTION_DEFINITION(IndexOutOfBoundsException, RuntimeException, Node);
CHILD_EXCEPTION_DEFINITION(NotFoundException, RuntimeException, Node);
CHILD_EXCEPTION_DEFINITION(DuplicateException, RuntimeException, Node);
CHILD_EXCEPTION_DEFINITION(OperatingSystemException, Exception, Node);
CHILD_EXCEPTION_DEFINITION(FileSystemException, OperatingSystemException, Node);

} // namespace Child
