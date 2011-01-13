#include "node/object/language/token.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Token, Object, Language);

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

    void Token::initRoot() {
    }
}

CHILD_END
