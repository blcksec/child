#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QtCore/QDebug>

template<typename T>
void p(T const &value) { QTextStream(stdout) << value << '\n'; }

#endif // TOOLBOX_H
