#include "child/language/token.h"

namespace Child {
    namespace Language {
        CHILD_DEFINITION(Token, Object, Language);

        void Token::initRoot() {
            // TODO
        }

        const char *Token::typesName[] = {
            "<NULL>",
            "<EOF>",
            "Name",
            "Label",
            "Operator",
            "Boolean",
            "Number",
            "Character",
            "Text",
            "LeftParenthesis",
            "RightParenthesis",
            "LeftBracket",
            "RightBracket",
            "LeftBrace",
            "RightBrace",
            "Semicolon",
            "Newline"
        };
    }
}
