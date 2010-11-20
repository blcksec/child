#ifndef CHILD_TOKEN_H
#define CHILD_TOKEN_H

#include "child/object.h"

#define CHILD_TOKEN(EXPRESSION) static_cast<Token *>(EXPRESSION)

namespace Child {
    class Token : public Object {
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

        static Token *root();
        static Token *fork(Node *world) { return(CHILD_TOKEN(world->child("Token"))->fork()); }
        static Token *fork(Node *world, const Type type, const QStringRef &sourceCodeRef) {
            return(CHILD_TOKEN(world->child("Token"))->fork(type, sourceCodeRef));
        }

        Type type;
        QStringRef sourceCodeRef;

        Token(const Type type = Eof, const QStringRef &sourceCodeRef = QStringRef()) :
                type(type), sourceCodeRef(sourceCodeRef) {}

        virtual Token *fork() {
            Token *tk = _fork(this);
            tk->type = type;
            tk->sourceCodeRef = sourceCodeRef;
            return(tk);
        }

        virtual Token *fork(const Type type, const QStringRef &sourceCodeRef) {
            Token *tk = _fork(this);
            tk->type = type;
            tk->sourceCodeRef = sourceCodeRef;
            return(tk);
        }

        static const QString typeName(const Type type) { return(typesName[type]); }
        const QString typeName() const { return(Token::typeName(type)); }
        const QString text() const { return(sourceCodeRef.toString()); }
        virtual const QString inspect() const { return(QString("%1: '%2'").arg(typeName()).arg(text())); }
    private:
        static Token *_root;
    };
}

#endif // CHILD_TOKEN_H
