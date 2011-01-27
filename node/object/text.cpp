#include "node/object/text.h"
#include "node/object/language/interpreter.h"

CHILD_BEGIN

CHILD_DEFINE(Text, Element, Object);

void Text::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Text, init);

    CHILD_ADD_NATIVE_METHOD(Text, concatenate, +);
    CHILD_ADD_NATIVE_METHOD(Text, multiply, *);

    CHILD_ADD_NATIVE_METHOD(Text, uppercase);
    CHILD_ADD_NATIVE_METHOD(Text, lowercase);
    CHILD_ADD_NATIVE_METHOD(Text, capitalize);

    CHILD_ADD_NATIVE_METHOD(Text, size);
    CHILD_ADD_NATIVE_METHOD(Text, empty);

    CHILD_ADD_NATIVE_METHOD(Text, equal_to, ==);
    CHILD_ADD_NATIVE_METHOD(Text, compare, <=>);
}

Node *Text::run(Node *receiver) {
    Q_UNUSED(receiver);
    if(interpolableSlices()) {
        CHILD_PUSH_RUN(this);
        QString result = value();
        QString source;
        QString str;
        int offset = 0;
        foreach (IntPair slice, *interpolableSlices()) {
            source = result.mid(offset + slice.first + 1, slice.second - 2);
            str = !source.isEmpty() ? Interpreter::root()->runSourceCode("child:" + source)->toString() : "";
            result.replace(offset + slice.first, slice.second, str);
            offset += str.size() - slice.second;
        }
        return CHILD_TEXT(result);
    } else
        return this;
}

QString Text::unescapeSequence(const QString &source, QList<IntPair> *interpolableSlices) {
    QString result;
    int i = 0;
    QChar c;
    while(i < source.size()) {
        c = source.at(i);
        if(c == '{' && interpolableSlices) {
            int end = source.indexOf('}', i + 1);
            if(end == -1) CHILD_THROW(RuntimeException, "'}' not found after '{'");
            interpolableSlices->append(IntPair(result.size(), end - i + 1));
            result += source.mid(i, end - i + 1);
            i = end + 1;
        } else {
            if(c == '\\') {
                i++;
                if(i == source.size()) CHILD_THROW(RuntimeException, "invalid escape sequence: '\\'");
                c = source.at(i);
                switch(c.toAscii()) {
                case 't': c = '\t';  break;
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case '"': c = '"'; break;
                case '\'': c = '\''; break;
                case '{': c = '{'; break;
                case '\\': c = '\\'; break;
                default:
                    if(QString("01234567xu").contains(c, Qt::CaseInsensitive))
                        c = Text::unescapeSequenceNumber(source, i);
                    else
                        CHILD_THROW(RuntimeException, QString("unknown escape sequence: '\\%1'").arg(c));
                }
            }
            result += c;
            i++;
        }
    }
    return result;
}

QChar Text::unescapeSequenceNumber(const QString &source, int &i) {
    char type;
    QString allowedChars;
    short maxSize;
    QChar c = source.at(i);
    if(c == 'x' || c == 'X') {
        type = 'x';
        allowedChars = "0123456789abcdef";
        maxSize = 2;
    } else if(c == 'u' || c == 'U') {
        type = 'u';
        allowedChars = "0123456789abcdef";
        maxSize = 4;
    } else {
        type = 'o';
        allowedChars = "01234567";
        maxSize = 3;
        i--;
    }
    QString number = "";
    do {
        i++;
        if(i == source.size()) CHILD_THROW(RuntimeException, "invalid escape sequence number");
        c = source.at(i);
        if(!allowedChars.contains(c, Qt::CaseInsensitive)) { i--; break; }
        number += c;
    } while(number.size() < maxSize);
    if(number.isEmpty()) CHILD_THROW(RuntimeException, "invalid escape sequence number");
    bool ok;
    ushort code = type == 'o' ? number.toUShort(&ok, 8) : number.toUShort(&ok, 16);
    if(!ok || (type != 'u' && code > 0xFF)) CHILD_THROW(RuntimeException, "invalid number in escape sequence");
    return QChar(code);
}

CHILD_END
