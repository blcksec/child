#include "child/module.h"
#include "child/text.h"
#include "child/nativemethod.h"
#include "child/test.h"

using namespace Child;

int main() { // int argc, char *argv[]
    TestModule::run();
//    NativeMethod *meth;

//    Module *world = new Module;
//    world->setSlot("World", world);

//    Module *mod = new Module(NULL, world);
//    world->setSlot("Base", mod);

//    NativeMethod *nativeMethod = new NativeMethod(mod);
//    world->setSlot("NativeMethod", nativeMethod);

//    mod->setSlot("to_text", NativeMethod::create(world, CHILD_METHOD(Module::toText)));
//    mod->setSlot("print", NativeMethod::create(world, CHILD_METHOD(Module::print)));

//    Module *object = new Module(mod);
//    world->setSlot("Object", object);

//    Text *text = new Text(object);
//    world->setSlot("Text", text);
//    text->setSlot("upcase", NativeMethod::create(world, CHILD_METHOD(Text::upcase)));

//    Text *t = Text::create(world, "yo");
//    t->send("upcase")->send("print");

//    Module *own, *rcv, *val;
//    if(t->send("upcase")->findSlot("print", own, rcv, val)) {
//        p("Trouvé !");
//        rcv->print();
//    }

//    Module *m = text->slot("upcase");
//    p(m);
//    m->print();
}
