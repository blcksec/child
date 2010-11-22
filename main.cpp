#include "tests/runalltests.h"
#include "child/application.h"

using namespace Child;

int main() { // int argc, char *argv[]
    runAllTests();
    try {
        Application *app = Application::root();
        app->init();
        SourceCode *source = app->loadSourceCode("../child/examples/test.child");
        p(source->inspect());
        p(Application::root()->className());
//        p(app->blocks()->inspect());
//        app->lexer()->setSource(&source);
//        app->lexer()->setFilename("lexertest.child");
//        p(escapeTabsAndNewlines(app->lexer()->toString()).toUtf8());

//        QString source = "body: a * 2++ + b * -3";
//        app->lexer()->setSource(&source);
//        p(app->parser()->run());

    //    QString source = "[a, b] = [c, d]";
    //    app->lexer()->setSource(&source);
    //    p(escapeTabsAndNewlines(app->lexer()->toString()).toUtf8());

    //    QString source = readTextFile("../child/examples/lexertest.child");
    //    app->lexer()->setSource(&source);
    //    app->lexer()->setFilename("lexertest.child");
    //    p(escapeTabsAndNewlines(app->lexer()->toString()).toUtf8());

//        QString source = readTextFile("../child/examples/lexertest.child");
//        app->lexer()->setSource(&source);
//        app->lexer()->setFilename("lexertest.child");
//        for(int i = 0; i < 1000; i++) {
//            bool eofFound = false;
//            while(!eofFound) {
//                const Token *token = app->lexer()->nextToken();
//                if(token->type == Token::Eof) eofFound = true;
//                delete token;
//            }
//            app->lexer()->rewind();
//        }
    } catch(Exception e) {
        qDebug() << e.message().toUtf8();
    }
}
