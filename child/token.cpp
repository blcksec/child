#include "child/token.h"

namespace Child {
    Token *Token::_root = Token::root();

    Token *Token::root() {
        if(!_root) {
            _root = new Token;
            _root->setOrigin(Object::root());
            _root->addParent("Token", Object::root());
        }
        return(_root);
    }

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
