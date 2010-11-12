#include "child/toolbox.h"

QString readTextFile(const QString &name) {
    QFile file(name);
    if(!file.open(QIODevice::ReadOnly)) qFatal("File not found");
    QString text = QTextStream(&file).readAll();
    file.close();
    return(text);
}
