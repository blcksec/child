#include "child/token.h"

namespace Child {
    CHILD_IMPLEMENTATION(Token, Object);

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
