//#include "tests/runalltests.h"
#include "child/parser.h"

using namespace Child;

int main() { // int argc, char *argv[]
//    runAllTests();
    try {
        Parser parser;
        parser.test();
    } catch(Exception e) {
        qDebug() << e.message.toUtf8();
    }
}
