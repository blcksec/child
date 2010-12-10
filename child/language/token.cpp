#include "child/language/token.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINITION(Token, Object);

    const char *Token::typeNames[] = {
        "<NULL>",
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
        "Semicolon",
        "Newline"
    };
}

CHILD_END
