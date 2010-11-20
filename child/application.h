#ifndef CHILD_APPLICATION_H
#define CHILD_APPLICATION_H

#include "child/object.h"
#include "child/operatortable.h"
#include "child/lexer.h"
#include "child/parser.h"

#define CHILD_APPLICATION(EXPRESSION) static_cast<Application *>(EXPRESSION)

namespace Child {
    class Application : public Object {
    public:
        static Application *root();
        static Application *fork(Node *world) { return(CHILD_APPLICATION(world->child("Application"))->fork()); }

        Application() : _operatorTable(NULL), _lexer(NULL), _parser(NULL) {}

        virtual ~Application() {
            delete _operatorTable;
            delete _lexer;
            delete _parser;
        }

        virtual Application *fork() { notYetImplemented(); return(NULL); }

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
    private:
        static Application *_root;

        OperatorTable *_operatorTable;
        Lexer *_lexer;
        Parser *_parser;
    };
}

#endif // CHILD_APPLICATION_H
