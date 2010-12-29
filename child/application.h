#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "child/language/operatortable.h"
#include "child/language/sourcecode.h"
#include "child/language/lexer.h"
//#include "child/language/parser.h"

CHILD_BEGIN

using namespace Language;

#define CHILD_APPLICATION(ARGS...) \
new Application(Node::context()->child("Object", "Application"), ##ARGS)

class Application : public Object {
    CHILD_DECLARE(Application, Object);
public:
    explicit Application(const Node *origin) :
        Object(origin), _sourceCodes(NULL), _operatorTable(NULL), _lexer(NULL), _parser(NULL) {}

    static void initRoot() { Object::root()->addChild("Application", root()); }
    virtual Application *fork() const { CHILD_TODO; return new Application(this); }

    void init() {
        _sourceCodes = CHILD_SOURCE_CODE_DICTIONARY();
        initOperatorTable();
        _lexer = CHILD_LEXER();
        addChild("lexer", _lexer);
        _parser = CHILD_PARSER();
        addChild("parser", _parser);
    }

    void initOperatorTable();

    SourceCodeDictionary *sourceCodes() const { return _sourceCodes; }
    OperatorTable *operatorTable() const { return _operatorTable; }
    Lexer *lexer() const { return _lexer; }
    Parser *parser() const { return _parser; }

    SourceCode *loadSourceCode(QString url) {
        url = QFileInfo(url).absoluteFilePath();
        SourceCode *source;
        if(!(source = sourceCodeIsAlreadyLoaded(url))) {
            source = CHILD_SOURCE_CODE(url);
            sourceCodes()->set(CHILD_TEXT(url), source);
        }
        return source;
    }

    SourceCode *sourceCodeIsAlreadyLoaded(QString url) {
        url = QFileInfo(url).absoluteFilePath();
        return sourceCodes()->hasKey(CHILD_TEXT(url));
    }
private:
    SourceCodeDictionary *_sourceCodes;
    OperatorTable *_operatorTable;
    Lexer *_lexer;
    Parser *_parser;
};

CHILD_END

#endif // CHILD_APPLICATION_H
