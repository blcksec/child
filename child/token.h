#ifndef TOKEN_H
#define TOKEN_H

#include <QtCore/QString>

namespace Child {
    class Token {
    public:
        enum Type {
            Eof,
            Name,
            Label,
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
            Semicolon,
            Newline
        };

        static const char *typesName[];

        Type type;
        QStringRef textRef;
        int column;
        int line;

        Token(const Type type = Eof, const QStringRef &textRef = QStringRef(), const int column = 0,const int line = 0) :
                type(type), textRef(textRef), column(column), line(line) {}

        static const QString typeName(const Type type) { return(typesName[type]); }
        const QString typeName() const { return(Token::typeName(type)); }
        const QString text() const { return(textRef.toString()); }
        const QString toString() const { return(QString("%1: '%2'").arg(typeName()).arg(text())); }
    };
}

#endif // TOKEN_H
