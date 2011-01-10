#include "child/language/token.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Token, Object);

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
        Language::root()->addChild("Token", this);
    }
}

CHILD_END
