#ifndef CHILD_TOKEN_H
#define CHILD_TOKEN_H

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
        QStringRef sourceCodeRef;

        Token(const Type type = Eof, const QStringRef &sourceCodeRef = QStringRef()) :
                type(type), sourceCodeRef(sourceCodeRef) {}

        static const QString typeName(const Type type) { return(typesName[type]); }
        const QString typeName() const { return(Token::typeName(type)); }
        const QString text() const { return(sourceCodeRef.toString()); }
        const QString toString() const { return(QString("%1: '%2'").arg(typeName()).arg(text())); }
    };
}

#endif // CHILD_TOKEN_H
