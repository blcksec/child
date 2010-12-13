#include <QtCore/QFile>

#include "child/exception.h"

CHILD_BEGIN

QString readTextFile(const QString &name) {
    QFile file(name);
    if(!file.open(QIODevice::ReadOnly)) CHILD_THROW(FileSystemException, "file not found");
    QString text = QTextStream(&file).readAll();
    file.close();
    return text;
}

QString concatenateStrings(const QString &first, const QString &separator, const QString &second) {
    QString str = first;
    if(!first.isEmpty() && !second.isEmpty()) str += separator;
    str += second;
    return str;
}

QString escapeTabsAndNewlines(QString text) {
    text.replace('\r', "\\r");
    text.replace('\n', "\\n");
    text.replace('\t', "\\t");
    return text;
}

QString extractLine(const QString &text, int requestedLine) {
    int line = 1;
    int pos = 0;
    while(pos != -1) {
        if(line == requestedLine) {
            int end = text.indexOf('\r', pos);
            if(end == -1) end = text.indexOf('\n', pos);
            if(end != -1)
                return text.mid(pos, end - pos);
            else
                return text.mid(pos) + ""; // Force non-null string
        }
        int pos2 = text.indexOf('\r', pos);
        if(pos2 != -1) {
            line++;
            pos = pos2 + 1;
            if(text.at(pos) == '\n') pos++; // Windows CR+LF case
        } else {
            pos = text.indexOf('\n', pos);
            if(pos != -1) {
                line++;
                pos++;
            }
        }
    }
    return QString();
}

const bool computeColumnAndLineForPosition(const QString &text, const int position, int &column, int &line) {
    line = 1;
    int pos = 0;
    int previousPos = 0;
    bool crFound;
    while(true) {
        pos = text.indexOf('\r', previousPos);
        crFound = pos != -1;
        if(!crFound) pos = text.indexOf('\n', previousPos);
        if(pos == -1) pos = text.size();
        if(pos >= position) {
            column = position - previousPos + 1;
            return true;
        } else if(pos < text.size()) {
            line++;
            pos++;
            if(crFound && pos < text.size() && text.at(pos) == '\n') pos++; // Windows CR+LF case
            previousPos = pos;
        } else break;
    }
    return false;
}

#define CHILD_THROW_FUNCTION(EXCEPTION) \
void throw##EXCEPTION(const QString &message, const QString &file, const int line, const QString &function) { \
    throw EXCEPTION##Pointer(new EXCEPTION(Node::context()->child(#EXCEPTION), message, file, line, function)); \
}

CHILD_THROW_FUNCTION(RuntimeException);
CHILD_THROW_FUNCTION(NullPointerException);
CHILD_THROW_FUNCTION(TypecastException);

CHILD_END
