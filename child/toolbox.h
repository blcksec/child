#ifndef CHILD_TOOLBOX_H
#define CHILD_TOOLBOX_H

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include "QtCore/QTextCodec"

template<typename T>
void p(T const &value) { QTextStream(stdout) << value << '\n'; }

QString readTextFile(const QString &name);
QString escapeTabsAndNewlines(QString text);
QString extractLine(const QString &text, int requestedLine);
const bool computeColumnAndLineForPosition(const QString &text, const int position, int &column, int &line);

#endif // CHILD_TOOLBOX_H
