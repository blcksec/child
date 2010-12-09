#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "child/language/operatortable.h"
#include "child/language/sourcecode.h"
#include "child/language/lexer.h"
//#include "child/language/parser.h"

CHILD_BEGIN

using namespace Language;

CHILD_PTR_DECLARATION(Application, Object);

#define CHILD_APPLICATION(ARGS...) \
ApplicationPtr(new Application(Node::context()->child("Object", "Application"), ##ARGS))

class Application : public Object {
    CHILD_DECLARATION(Application, Object);
public:
    Application(const NodePtr &origin) : Object(origin) {}

    static void initRoot() { Object::root()->addChild("Application", root()); }
    virtual NodePtr fork() const { CHILD_TODO; return new Application(this); }

    void init() {
        _sourceCodes = CHILD_SOURCE_CODE_DICTIONARY();
        initOperatorTable();
        _lexer = CHILD_LEXER();
        addChild("lexer", _lexer);
        _parser = CHILD_PARSER();
        addChild("parser", _parser);
    }

    void initOperatorTable();

    SourceCodeDictionaryPtr sourceCodes() const { return _sourceCodes; }
    OperatorTablePtr operatorTable() const { return _operatorTable; }
    LexerPtr lexer() const { return _lexer; }
    ParserPtr parser() const { return _parser; }

    SourceCodePtr loadSourceCode(QString url) {
        url = QFileInfo(url).absoluteFilePath();
        SourceCodePtr source;
        if(!(source = sourceCodeIsAlreadyLoaded(url))) {
            source = CHILD_SOURCE_CODE(url);
            sourceCodes()->set(CHILD_TEXT(url), source);
        }
        return source;
    }

    SourceCodePtr sourceCodeIsAlreadyLoaded(QString url) {
        url = QFileInfo(url).absoluteFilePath();
        return sourceCodes()->hasKey(CHILD_TEXT(url));
    }
private:
    SourceCodeDictionaryPtr _sourceCodes;
    OperatorTablePtr _operatorTable;
    LexerPtr _lexer;
    ParserPtr _parser;
};

CHILD_PTR_DEFINITION(Application, Object);

CHILD_END

#endif // CHILD_APPLICATION_H
