#include "child/toolbox.h"

QString readTextFile(const QString &name) {
    QFile file(name);
    if(!file.open(QIODevice::ReadOnly)) qFatal("File not found");
    QString text = QTextStream(&file).readAll();
    file.close();
    return(text);
}

QString extractLine(const QString &text, int requestedLine) {
    int line = 1;
    int position = 0;
    while(position != -1) {
        if(line == requestedLine) {
            int end = text.indexOf('\r', position);
            if(end == -1) end = text.indexOf('\n', position);
            if(end != -1)
                return(text.mid(position, end - position));
            else
                return(text.mid(position) + ""); // Force non-null string
        }
        int pos = text.indexOf('\r', position);
        if(pos != -1) {
            line++;
            position = pos + 1;
            if(text.at(position) == '\n') position++; // Windows CR+LF case
        } else {
            position = text.indexOf('\n', position);
            if(position != -1) {
                line++;
                position++;
            }
        }
    }
    return(QString());
}

QString escapeTabsAndNewlines(QString text) {
    text.replace('\r', "\\r");
    text.replace('\n', "\\n");
    text.replace('\t', "\\t");
    return(text);
}
