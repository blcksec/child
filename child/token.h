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
            Comment,
            LeftParenthesis,
            RightParenthesis,
            LeftBracket,
            RightBracket,
            LeftBrace,
            RightBrace,
            Comma,
            Period,
            Semicolon,
            Colon
        };

        static const char *typesName[];

        Type type;
        QString text;

        Token(const Type type, const QString &text) : type(type), text(text) {}

        const QString toString() { return(QString("%1: '%2'").arg(typesName[type]).arg(text)); }
    };
}

#endif // TOKEN_H
