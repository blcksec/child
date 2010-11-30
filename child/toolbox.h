#ifndef CHILD_TOOLBOX_H
#define CHILD_TOOLBOX_H

#include <QtCore/QTextStream>

#define P(VALUE) \
QTextStream(stdout) << VALUE << '\n';

#define PP P(Q_FUNC_INFO);

namespace Child {

QString readTextFile(const QString &name);
QString escapeTabsAndNewlines(QString text);
QString extractLine(const QString &text, int requestedLine);
const bool computeColumnAndLineForPosition(const QString &text, const int position, int &column, int &line);

} // namespace Child

#endif // CHILD_TOOLBOX_H
