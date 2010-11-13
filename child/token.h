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
            Colon
        };

        static const char *typesName[];

        Type type;
        QString text;
        int position;
        int column;
        int line;

        Token(const Type type = Eof, const QString &text = "", const int position = -1, const int column = 0,const int line = 0) :
                type(type), text(text), position(position), column(column), line(line) {}

        const QString toString() { return(QString("%1: '%2'").arg(typesName[type]).arg(text)); }
    };
}

#endif // TOKEN_H
