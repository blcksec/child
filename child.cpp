#include <QtCore/QFile>

#include "child.h"
#include "node/exception.h"
#include "node/object/language/primitive.h"
#include "node/object/message.h"
#include "node/object/language/interpreter.h"

CHILD_BEGIN

void init() {
    foreach(Root root, roots())
        root.node->setNodeName(root.name);

    Interpreter *interpreter = Interpreter::root();
    QString path = "../child/";
    foreach(Root root, roots()) {
        Node *node = root.node;
        QString childFile = path + node->nodePath() + node->nodeName().toLower() + ".child";
        if(QFileInfo(childFile).exists()) {
            interpreter->loadSourceCode(childFile)->run();
        }
    }
    interpreter->testSuite()->run();
    P(QString("All tests passed (%1 sections, %2 assertions)").
      arg(interpreter->testSuite()->size()).arg(Node::passedAssertionCount()));
}

QList<Root> &roots() {
    static QList<Root> _roots;
    return _roots;
}

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

bool computeColumnAndLineForPosition(const QString &text, const int position, int &column, int &line) {
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

QString preferSecondArgumentIfNotEmpty(const QString &a, const QString &b) {
    return b.isEmpty() ? a : b;
}

#define CHILD_DEFINE_THROW_FUNCTION(EXCEPTION) \
void throw##EXCEPTION(const QString &message, const QString &file, const int line, const QString &function) { \
    throw EXCEPTION(context()->child(#EXCEPTION), message, file, line, function); \
}

CHILD_DEFINE_THROW_FUNCTION(RuntimeException);
CHILD_DEFINE_THROW_FUNCTION(NullPointerException);
CHILD_DEFINE_THROW_FUNCTION(NotFoundException);
CHILD_DEFINE_THROW_FUNCTION(TypecastException);
CHILD_DEFINE_THROW_FUNCTION(ConversionException);

CHILD_END
