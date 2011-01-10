#ifndef CHILD_INTERPRETER_H
#define CHILD_INTERPRETER_H

#include "child/language/operatortable.h"
#include "child/language/sourcecode.h"
#include "child/language/lexer.h"
#include "child/language/parser.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_INTERPRETER(ARGS...) \
    new Interpreter(Node::context()->child("Object", "Language", "Interpreter"), ##ARGS)

    class Interpreter : public Object {
        CHILD_DECLARE(Interpreter, Object);
    public:
        explicit Interpreter(Node *origin) :
            Object(origin), _operatorTable(NULL), _lexer(NULL), _parser(NULL), _sourceCodes(NULL) {}

        CHILD_FORK_METHOD(Interpreter); // TODO

        void initOperatorTable();

        OperatorTable *operatorTable() const { return _operatorTable; }
        Lexer *lexer() const { return _lexer; }
        Parser *parser() const { return _parser; }

        SourceCodeDictionary *sourceCodes() const { return _sourceCodes; }
        SourceCode *loadSourceCode(QString url);
        SourceCode *sourceCodeIsAlreadyLoaded(QString url);
    private:
        OperatorTable *_operatorTable;
        Lexer *_lexer;
        Parser *_parser;
        SourceCodeDictionary *_sourceCodes;
    };
}

CHILD_END

#endif // CHILD_INTERPRETER_H
