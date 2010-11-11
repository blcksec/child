#include "lexer.h"

namespace Child {
    const char *Lexer::tokenTypesName[] = {
        "<EOF>",
        "Name",
        "Operator",
        "Number",
        "Text"
    };
    const QString Lexer::allowedCharsInNames = "_!?";
    const QString Lexer::firstCharsInOperators = "@#&!-^$*%<>?/\\+=";
}
