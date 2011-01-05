#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "child/language/operatortable.h"
#include "child/language/sourcecode.h"
#include "child/language/lexer.h"
#include "child/language/parser.h"

CHILD_BEGIN

#define CHILD_APPLICATION(ARGS...) \
new Application(Node::context()->child("Object", "Application"), ##ARGS)

class Application : public Object {
    CHILD_DECLARE(Application, Object);
public:
    explicit Application(Node *origin) :
        Object(origin), _sourceCodes(NULL), _operatorTable(NULL), _lexer(NULL), _parser(NULL) {}

    CHILD_FORK_METHOD(Application); // TODO

    void init();
    void initOperatorTable();

    SourceCodeDictionary *sourceCodes() const { return _sourceCodes; }
    OperatorTable *operatorTable() const { return _operatorTable; }
    Lexer *lexer() const { return _lexer; }
    Parser *parser() const { return _parser; }

    SourceCode *loadSourceCode(QString url);
    SourceCode *sourceCodeIsAlreadyLoaded(QString url);
private:
    SourceCodeDictionary *_sourceCodes;
    OperatorTable *_operatorTable;
    Lexer *_lexer;
    Parser *_parser;
};

CHILD_END

#endif // CHILD_APPLICATION_H
