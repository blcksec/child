#ifndef CHILD_GLOBAL_H
#define CHILD_GLOBAL_H

#include <QtCore/QTextStream>

//#define CHILD_IS_NAMESPACED

#ifdef CHILD_IS_NAMESPACED
#define CHILD_BEGIN namespace Child {
#define CHILD_END }
#else
#define CHILD_BEGIN
#define CHILD_END
#endif

#ifdef CHILD_IS_NAMESPACED
#define CHILD_USE using namespace Child;
#else
#define CHILD_USE
#endif

CHILD_BEGIN

#define P(VALUE) \
QTextStream(stdout) << VALUE << '\n';

#define PP P(Q_FUNC_INFO);

#define CHILD_THROW_RUNTIME_EXCEPTION(MESSAGE) \
throwRuntimeException(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)

#define CHILD_THROW_NULL_POINTER_EXCEPTION(MESSAGE) \
throwNullPointerException(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)

#define CHILD_THROW_TYPECAST_EXCEPTION(MESSAGE) \
throwTypecastException(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)

#define CHILD_THROW_CONVERSION_EXCEPTION(MESSAGE) \
throwConversionException(MESSAGE, __FILE__, __LINE__, Q_FUNC_INFO)

typedef long long int HugeInteger;
typedef unsigned long long int HugeUnsignedInteger;

QString readTextFile(const QString &name);
QString concatenateStrings(const QString &first, const QString &separator, const QString &second);
QString escapeTabsAndNewlines(QString text);
QString extractLine(const QString &text, int requestedLine);
const bool computeColumnAndLineForPosition(const QString &text, const int position, int &column, int &line);
QString preferSecondArgumentIfNotEmpty(const QString &a, const QString &b);

void throwRuntimeException(const QString &message = "", const QString &file = "", const int line = 0, const QString &function = "");
void throwNullPointerException(const QString &message = "", const QString &file = "", const int line = 0, const QString &function = "");
void throwTypecastException(const QString &message = "", const QString &file = "", const int line = 0, const QString &function = "");
void throwConversionException(const QString &message = "", const QString &file = "", const int line = 0, const QString &function = "");

CHILD_END

#endif // CHILD_GLOBAL_H
