#ifndef TOKEN_H
#define TOKEN_H

#include "child/node.h"

namespace Child {
//    class Lexer;

    class Token {
    public:
        int type;
        QString text;

        Token(const int type, const QString &text) : type(type), text(text) {}
    };
}

#endif // TOKEN_H
