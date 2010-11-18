#include "child/token.h"

namespace Child {
    const char *Token::typesName[] = {
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
