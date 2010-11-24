#ifndef CHILD_TOKEN_H
#define CHILD_TOKEN_H

#include "child/object.h"
#include "child/language.h"

namespace Child {
    namespace Language {
        class Token : public Object {
            CHILD_DECLARATION(Token, Object, Language);
        public:
            enum Type {
                Null,
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

            static Token *fork(Node *world, const Type type, const QStringRef &sourceCodeRef) {
                Token *f = Token::fork(world);
                f->type = type;
                f->sourceCodeRef = sourceCodeRef;
                return f;
            }

            Type type;
            QStringRef sourceCodeRef;

            Token(const Type type = Null, const QStringRef &sourceCodeRef = QStringRef()) :
                    type(type), sourceCodeRef(sourceCodeRef) {}

            virtual void initFork() {
                Token *orig = Token::as(origin());
                type = orig->type;
                sourceCodeRef = orig->sourceCodeRef;
            }

            static const QString typeName(const Type type) { return typesName[type]; }
            const QString typeName() const { return Token::typeName(type); }
            const QString text() const { return sourceCodeRef.toString(); }
            virtual const QString inspect() const { return QString("%1: '%2'").arg(typeName()).arg(text()); }
        private:
        };
    }
}

#endif // CHILD_TOKEN_H
