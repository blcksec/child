#ifndef LEXER_H
#define LEXER_H

#include "child/node.h"

namespace Child {
    class Lexer {
    public:
        Lexer() {}

        void run() {
            int cursor = 0;

        }

        void setSource(const QString &source) { _source = source; }
        const QString &source() const { return _source; }

        void test() {
            setSource("i = 5");
            run();
        }

    private:
        QString _source;
    };
}

#endif // LEXER_H
