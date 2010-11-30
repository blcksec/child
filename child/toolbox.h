#ifndef CHILD_TOOLBOX_H
#define CHILD_TOOLBOX_H

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QTextStream>
#include "QtCore/QTextCodec"

namespace Child {
    typedef unsigned long long int HugeUnsignedInteger;

    template<typename T>
    void p(T const &value) { QTextStream(stdout) << value << '\n'; }

    QString readTextFile(const QString &name);
    QString escapeTabsAndNewlines(QString text);
    QString extractLine(const QString &text, int requestedLine);
    const bool computeColumnAndLineForPosition(const QString &text, const int position, int &column, int &line);
    void notYetImplemented();
}

#endif // CHILD_TOOLBOX_H
