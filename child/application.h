#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "child/object.h"
#include "child/language/lexer.h"
#include "child/language/operatortable.h"
#include "child/language/parser.h"
#include "child/language/sourcecode.h"

namespace Child {
    class Application : public Object {
        CHILD_DECLARATION(Application, Object, Object);
    public:
        Application() : _operatorTable(NULL), _lexer(NULL), _parser(NULL), _sourceCodes(NULL) {}

        virtual ~Application() {
            delete _operatorTable;
            delete _lexer;
            delete _parser;
            delete _sourceCodes;
        }

        void init() {
            initOperatorTable();
            _lexer = Language::Lexer::fork(this);
            _lexer->setOperatorTable(operatorTable());
            _parser = Language::Parser::fork(this);
            _parser->setLexer(_lexer);
        }

        void initOperatorTable();

        Language::OperatorTable *operatorTable() const { return _operatorTable; }
        Language::Lexer *lexer() const { return _lexer; }
        Language::Parser *parser() const { return _parser; }


        Dictionary *sourceCodes() const { return _sourceCodes; }

        Language::SourceCode *loadSourceCode(QString url) {
            url = QFileInfo(url).absoluteFilePath();
            if(sourceCodeIsAlreadyLoaded(url)) return Language::SourceCode::as(sourceCodes()->get(url));
            Language::SourceCode *source = Language::SourceCode::fork(this);
            source->load(url);
            source->parse(parser());
            if(!sourceCodes()) _sourceCodes = Dictionary::fork(this);
            sourceCodes()->set(url, source);
            return source;
        }

        const bool sourceCodeIsAlreadyLoaded(QString URL) {
            URL = QFileInfo(URL).absoluteFilePath();
            if(!sourceCodes()) return false;
            return sourceCodes()->hasKey(URL);
        }
    private:
        Language::OperatorTable *_operatorTable;
        Language::Lexer *_lexer;
        Language::Parser *_parser;
        Dictionary *_sourceCodes;
    };
}

#endif // CHILD_APPLICATION_H
