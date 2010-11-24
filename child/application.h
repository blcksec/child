#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "child/object.h"
#include "child/operatortable.h"
#include "child/language/lexer.h"
#include "child/parser.h"
#include "child/sourcecode.h"

namespace Child {
    class Application : public Object {
        CHILD_DECLARATION(Application);
    public:
        Application() : _operatorTable(NULL), _lexer(NULL), _parser(NULL) {}

        virtual ~Application() {
            delete _operatorTable;
            delete _lexer;
            delete _parser;
        }

        void init() {
            initOperatorTable();
            _lexer = Lexer::fork(this);
            _lexer->setOperatorTable(operatorTable());
            _parser = Parser::fork(this);
            _parser->setLexer(_lexer);
        }

        void initOperatorTable();

        OperatorTable *operatorTable() const { return(_operatorTable); }
        Lexer *lexer() const { return(_lexer); }
        Parser *parser() const { return(_parser); }


        Dictionary *sourceCodes() const { return(_sourceCodes); }

        SourceCode *loadSourceCode(QString url) {
            url = QFileInfo(url).absoluteFilePath();
            if(sourceCodeIsAlreadyLoaded(url)) return(SourceCode::as(sourceCodes()->get(url)));
            SourceCode *source = SourceCode::fork(this);
            source->load(url);
            source->parse(parser());
            if(!sourceCodes()) _sourceCodes = Dictionary::fork(this);
            sourceCodes()->set(url, source);
            return(source);
        }

        const bool sourceCodeIsAlreadyLoaded(QString URL) {
            URL = QFileInfo(URL).absoluteFilePath();
            if(!sourceCodes()) return(false);
            return(sourceCodes()->hasKey(URL));
        }
    private:
        OperatorTable *_operatorTable;
        Lexer *_lexer;
        Parser *_parser;
        Dictionary *_sourceCodes;
    };
}

#endif // CHILD_APPLICATION_H
