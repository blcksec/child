#ifndef CHILD_LANGUAGE_TOKEN_H
#define CHILD_LANGUAGE_TOKEN_H

#include "node/language.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_TOKEN(ARGS...) \
    new Language::Token(Node::context()->child("Object", "Language", "Token"), ##ARGS)

    class Token : public Object {
        CHILD_DECLARE(Token, Object);
    public:
        enum Type {
            Null,
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
            Semicolon,
            Newline
        };

        static const char *typeNames[];

        Type type;
        QStringRef sourceCodeRef;

        explicit Token(Node *origin, const Type type = Null, const QStringRef &sourceCodeRef = QStringRef()) :
            Object(origin), type(type), sourceCodeRef(sourceCodeRef) {}

        CHILD_FORK_METHOD(Token, type, sourceCodeRef);

        const QString typeName() const { return Token::typeName(type); }
        const QString text() const { return sourceCodeRef.toString(); }

        static const QString typeName(const Type type) { return typeNames[type]; }

        virtual QString toString(bool debug = false, short level = 0) const {
            Q_UNUSED(level);
            return QString("%1: '%2'").arg(typeName(), debug ? escapeTabsAndNewlines(text()) : text());
        }
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_TOKEN_H
