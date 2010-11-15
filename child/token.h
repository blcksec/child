#ifndef TOKEN_H
#define TOKEN_H

#include <QtCore/QString>

namespace Child {
    class Token {
    public:
        enum Type {
            Eof,
            Name,
            Operator,
            Boolean,
            Number,
            Character,
            Text,
            LeftParenthesis,
            RightParenthesis,
            LeftBracket,
            RightBracket,
            LeftBrace,
            RightBrace,
            Comma,
            Semicolon,
            Colon,
            Newline
        };

        static const char *typesName[];

        Type type;
        QStringRef textRef;
        int column;
        int line;

        Token(const Type type = Eof, const QStringRef &textRef = QStringRef(), const int column = 0,const int line = 0) :
                type(type), textRef(textRef), column(column), line(line) {}

        const QString text() { return(textRef.toString()); }

        const QString toString() { return(QString("%1: '%2'").arg(typesName[type]).arg(text())); }
    };
}

#endif // TOKEN_H
