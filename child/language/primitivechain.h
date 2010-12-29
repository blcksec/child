#ifndef CHILD_LANGUAGE_PRIMITIVE_CHAIN_H
#define CHILD_LANGUAGE_PRIMITIVE_CHAIN_H

#include "child/list.h"
#include "child/language/primitive.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_PRIMITIVE_CHAIN(ARGS...) \
    new Language::PrimitiveChain(Node::context()->child("Object", "Language", "PrimitiveChain"), ##ARGS)

    class PrimitiveChain : public GenericList<Primitive *> {
        CHILD_DECLARE(PrimitiveChain, List);
    public:
        explicit PrimitiveChain(const Node *origin) :
            GenericList<Primitive *>(origin) {}

        PrimitiveChain(const Node *origin, Primitive *primitive) :
            GenericList<Primitive *>(origin, primitive) {}

        PrimitiveChain(const Node *origin, Node *node) :
            GenericList<Primitive *>(origin, CHILD_PRIMITIVE(node)) {}

        static void initRoot() { Language::root()->addChild("PrimitiveChain", root()); }

        virtual PrimitiveChain *fork() const {
            PrimitiveChain *chain = new PrimitiveChain(this);
            chain->initFork();
            return chain;
        }

        virtual Node *run(Node *receiver = context()) {
            Node *result = NULL;
            Node *currentReceiver = receiver;
            Iterator i(this);
            while(Primitive *primitive = i.next()) {
                result = primitive->run(currentReceiver);
                currentReceiver = result;
            }
            return result;
        }

        Node *runExceptLast(Node *receiver = context()) {
            Node *result = receiver;
            Iterator i(this);
            while(Primitive *primitive = i.next())
                if(i.hasNext()) result = primitive->run(result);
            return result;
        }

        virtual QString toString(bool debug = false, short level = 0) const { return join(" ", "", "", debug, level); }
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_PRIMITIVE_CHAIN_H
