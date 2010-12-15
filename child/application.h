#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "child/language/operatortable.h"
#include "child/language/sourcecode.h"
#include "child/language/lexer.h"
//#include "child/language/parser.h"

CHILD_BEGIN

using namespace Language;

CHILD_POINTER_DECLARE(Application, Object);

#define CHILD_APPLICATION(ARGS...) \
ApplicationPointer(new Application(Node::context()->child("Object", "Application"), ##ARGS))

class Application : public Object {
    CHILD_DECLARE(Application, Object);
public:
    Application(const Pointer &origin) : Object(origin) {}

    static void initRoot() { Object::root()->addChild("Application", root()); }
    virtual Pointer fork() const { CHILD_TODO; return new Application(this); }

    void init() {
        _sourceCodes = CHILD_SOURCE_CODE_DICTIONARY();
        initOperatorTable();
        _lexer = CHILD_LEXER();
        addChild("lexer", _lexer);
        _parser = CHILD_PARSER();
        addChild("parser", _parser);
    }

    void initOperatorTable();

    SourceCodeDictionaryPointer sourceCodes() const { return _sourceCodes; }
    OperatorTablePointer operatorTable() const { return _operatorTable; }
    LexerPointer lexer() const { return _lexer; }
    ParserPointer parser() const { return _parser; }

    SourceCodePointer loadSourceCode(QString url) {
        url = QFileInfo(url).absoluteFilePath();
        SourceCodePointer source;
        if(!(source = sourceCodeIsAlreadyLoaded(url))) {
            source = CHILD_SOURCE_CODE(url);
            sourceCodes()->set(CHILD_TEXT(url), source);
        }
        return source;
    }

    SourceCodePointer sourceCodeIsAlreadyLoaded(QString url) {
        url = QFileInfo(url).absoluteFilePath();
        return sourceCodes()->hasKey(CHILD_TEXT(url));
    }
private:
    SourceCodeDictionaryPointer _sourceCodes;
    OperatorTablePointer _operatorTable;
    LexerPointer _lexer;
    ParserPointer _parser;
};

CHILD_POINTER_DEFINE(Application, Object);

CHILD_END

#endif // CHILD_APPLICATION_H
