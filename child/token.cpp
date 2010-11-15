#include "token.h"

namespace Child {
    const char *Token::typesName[] = {
        "<EOF>",
        "Name",
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
        "Comma",
        "Semicolon",
        "Colon",
        "Newline"
    };
}
