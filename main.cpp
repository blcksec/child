//#include "tests/runalltests.h"
//#include "child/application.h"
#include "child/node.h"

using namespace Child;

int main() { // int argc, char *argv[]
//    runAllTests();
//    try {
////        Application *app = Application::root();
////        app->init();
////        Language::SourceCode *source = app->loadSourceCode("../child/examples/test.child");
////        p(source->inspect());

////        Application *app = Application::root();
////        Text *k1 = Text::fork(app, "k");
////        Text *k2 = Text::fork(app, "k");
////        QHash<NodeRef, int> hash;
////        NodeRef r1(k1);
////        NodeRef r2(k2);
////        hash[r1] = 123;
////        hash[r2] = 125;
////        p(hash[r1]);
////        p(hash[r2]);
////        p(r1 == r2);

////        Application *app = Application::root();
////        List *l = List::fork(app);
////        Text *t1 = Text::fork(app, "t1");
////        Text *t2 = Text::fork(app, "t2");
////        l->append(t1);
////        l->append(t2);
////        foreach(Node *const n, *l) {
////            p(n->inspect());
////        }

////        p(app->blocks()->inspect());
////        app->lexer()->setSource(&source);
////        app->lexer()->setFilename("lexertest.child");
////        p(escapeTabsAndNewlines(app->lexer()->toString()).toUtf8());

////        QString source = "body: a * 2++ + b * -3";
////        app->lexer()->setSource(&source);
////        p(app->parser()->run());

//    //    QString source = "[a, b] = [c, d]";
//    //    app->lexer()->setSource(&source);
//    //    p(escapeTabsAndNewlines(app->lexer()->toString()).toUtf8());

//    //    QString source = readTextFile("../child/examples/lexertest.child");
//    //    app->lexer()->setSource(&source);
//    //    app->lexer()->setFilename("lexertest.child");
//    //    p(escapeTabsAndNewlines(app->lexer()->toString()).toUtf8());

////        QString source = readTextFile("../child/examples/lexertest.child");
////        app->lexer()->setSource(&source);
////        app->lexer()->setFilename("lexertest.child");
////        for(int i = 0; i < 1000; i++) {
////            bool eofFound = false;
////            while(!eofFound) {
////                const Token *token = app->lexer()->nextToken();
////                if(token->type == Token::Eof) eofFound = true;
////                delete token;
////            }
////            app->lexer()->rewind();
////        }
//    } catch(Exception e) {
//        qDebug() << e.message().toUtf8();
//    }
}
