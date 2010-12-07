#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "child/language/operatortable.h"
#include "child/language/sourcecode.h"
//#include "child/language/lexer.h"
//#include "child/language/parser.h"

CHILD_BEGIN

CHILD_PTR_DECLARATION(Application, Object);

#define CHILD_APPLICATION(ARGS...) \
ApplicationPtr(new Application(Node::findInContext("Object")->child("Application"), ##ARGS))

class Application : public Object {
    CHILD_DECLARATION(Application, Object);
public:
    Application(const NodePtr &origin) : Object(origin) {}

    static void initRoot() { Object::root()->addChild("Application", root()); }
    virtual NodePtr fork() const { CHILD_TODO; return new Application(this); }

    void init() {
        initOperatorTable();
//        _lexer = Language::Lexer::fork(this);
//        _lexer->setOperatorTable(operatorTable());
//        _parser = Language::Parser::fork(this);
//        _parser->setLexer(_lexer);
    }

    void initOperatorTable();

    Language::SourceCodeListPtr sourceCodes() const { return _sourceCodes; }
    Language::OperatorTablePtr operatorTable() const { return _operatorTable; }
//    Language::LexerPtr lexer() const { return _lexer; }
//    Language::ParserPtr parser() const { return _parser; }

//    Language::SourceCodePtr loadSourceCode(QString url) {
//        url = QFileInfo(url).absoluteFilePath();
//        if(sourceCodeIsAlreadyLoaded(url)) return Language::SourceCode::as(sourceCodes()->get(url));
//        Language::SourceCode *source = Language::SourceCode::fork(this);
//        source->load(url);
//        source->parse(parser());
//        if(!sourceCodes()) _sourceCodes = Dictionary::fork(this);
//        sourceCodes()->set(url, source);
//        return source;
//    }

//    const bool sourceCodeIsAlreadyLoaded(QString url) {
//        url = QFileInfo(url).absoluteFilePath();
//        if(!sourceCodes()) return false;
//        return sourceCodes()->hasKey(url);
//    }
private:
    Language::SourceCodeListPtr _sourceCodes;
    Language::OperatorTablePtr _operatorTable;
//    Language::LexerPtr _lexer;
//    Language::ParserPtr _parser;
};

CHILD_PTR_DEFINITION(Application, Object);

CHILD_END

#endif // CHILD_APPLICATION_H
