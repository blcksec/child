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
        "Comment",
        "LeftParenthesis",
        "RightParenthesis",
        "LeftBracket",
        "RightBracket",
        "LeftBrace",
        "RightBrace",
        "Comma",
        "Period",
        "Semicolon",
        "Colon"
    };
}
