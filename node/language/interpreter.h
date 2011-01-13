#ifndef CHILD_LANGUAGE_INTERPRETER_H
#define CHILD_LANGUAGE_INTERPRETER_H

#include "node/language/operatortable.h"
#include "node/language/lexer.h"
#include "node/language/parser.h"
#include "node/language/sourcecodedictionary.h"
#include "node/language/testsuite.h"

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

        TestSuite *testSuite() const { return TestSuite::cast(constCast(this)->child("test_suite")); }
        void setTestSuite(TestSuite *testSuite) { addOrSetChild("test_suite", testSuite); }
    private:
        OperatorTable *_operatorTable;
        Lexer *_lexer;
        Parser *_parser;
        SourceCodeDictionary *_sourceCodes;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_INTERPRETER_H
