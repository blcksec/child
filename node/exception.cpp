#include <QtCore/QFileInfo>

#include "node/exception.h"

CHILD_BEGIN

CHILD_EXCEPTION_DEFINITION(Exception, Node);

const QString Exception::report() const {
    QString str;
    if(!file.isEmpty()) str += QString("%1:").arg(QFileInfo(file).fileName());
    if(line != 0) str += QString("%1: ").arg(line);
    str += nodeName();
    if(!message.isEmpty()) str += QString(": %1").arg(message);
    if(!function.isEmpty()) str += QString(" in '%1'").arg(function);
    return str;
}

CHILD_EXCEPTION_DEFINITION(LexerException, Exception);
CHILD_EXCEPTION_DEFINITION(ParserException, Exception);
CHILD_EXCEPTION_DEFINITION(RuntimeException, Exception);
CHILD_EXCEPTION_DEFINITION(InterpreterException, RuntimeException);
CHILD_EXCEPTION_DEFINITION(ArgumentException, RuntimeException);
CHILD_EXCEPTION_DEFINITION(NullPointerException, RuntimeException);
CHILD_EXCEPTION_DEFINITION(IndexOutOfBoundsException, RuntimeException);
CHILD_EXCEPTION_DEFINITION(NotFoundException, RuntimeException);
CHILD_EXCEPTION_DEFINITION(DuplicateException, RuntimeException);
CHILD_EXCEPTION_DEFINITION(TypecastException, RuntimeException);
CHILD_EXCEPTION_DEFINITION(ConversionException, RuntimeException);
CHILD_EXCEPTION_DEFINITION(AssertionException, RuntimeException);
CHILD_EXCEPTION_DEFINITION(OperatingSystemException, Exception);
CHILD_EXCEPTION_DEFINITION(FileSystemException, OperatingSystemException);

CHILD_END
