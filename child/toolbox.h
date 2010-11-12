#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

template<typename T>
void p(T const &value) { QTextStream(stdout) << value << '\n'; }

QString readTextFile(const QString &name);

#endif // TOOLBOX_H
