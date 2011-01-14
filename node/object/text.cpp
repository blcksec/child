#include "node/object/text.h"

CHILD_BEGIN

CHILD_DEFINE(Text, Element, Object);

void Text::initRoot() {
    CHILD_NATIVE_METHOD_ADD(Text, init);
    CHILD_NATIVE_METHOD_ADD(Text, concatenate, +);
    CHILD_NATIVE_METHOD_ADD(Text, multiply, *);
    CHILD_NATIVE_METHOD_ADD(Text, uppercased);
    CHILD_NATIVE_METHOD_ADD(Text, uppercase_em, uppercase!);
    CHILD_NATIVE_METHOD_ADD(Text, lowercased);
    CHILD_NATIVE_METHOD_ADD(Text, lowercase_em, lowercase!);
    CHILD_NATIVE_METHOD_ADD(Text, equal_to, ==);
    CHILD_NATIVE_METHOD_ADD(Text, compare, <=>);
}

QString Text::unescapeSequence(const QString &source) {
    QString result;
    int i = 0;
    QChar c;
    while(i < source.size()) {
        c = source.at(i);
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
