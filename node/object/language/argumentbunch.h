#ifndef CHILD_LANGUAGE_ARGUMENT_BUNCH_H
#define CHILD_LANGUAGE_ARGUMENT_BUNCH_H

#include "node/object/bunch.h"
#include "node/object/language/argument.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_ARGUMENT_BUNCH(ARGS...) \
    new Language::ArgumentBunch(context()->child("Object", "Language", "ArgumentBunch"), ##ARGS)

    #define CHILD_CHECK_INPUT_SIZE(ARGS...) \
    ArgumentBunch::checkSpecifiedSize(message->inputs(false) ? message->inputs()->size() : 0, ##ARGS)

    #define CHILD_CHECK_QUESTION_MARK \
    if(!message->isQuestioned()) CHILD_THROW(InterpreterException, "missing question mark");

    #define CHILD_CHECK_EXCLAMATION_MARK \
    if(!message->isExclaimed()) CHILD_THROW(InterpreterException, "missing exclamation mark");

    #define CHILD_CHECK_QUESTION_OR_EXCLAMATION_MARK \
    if(!message->isQuestioned() && !message->isExclaimed()) \
        CHILD_THROW(InterpreterException, "missing question or exclamation mark");

    class ArgumentBunch : public GenericList<Argument *> {
        CHILD_DECLARE(ArgumentBunch, Bunch, Language);
    public:
        explicit ArgumentBunch(Node *origin) : GenericList<Argument *>(origin, true) {}

        ArgumentBunch(Node *origin, Argument *argument) :
            GenericList<Argument *>(origin, argument, true) {}

        ArgumentBunch(Node *origin, Argument *argument1, Argument *argument2) :
            GenericList<Argument *>(origin, argument1, argument2, true) {}

        ArgumentBunch(Node *origin, Node *argument) :
            GenericList<Argument *>(origin, CHILD_ARGUMENT(argument), true) {}

        ArgumentBunch(Node *origin, Node *argument1, Node *argument2) :
            GenericList<Argument *>(
                origin, CHILD_ARGUMENT(argument1), CHILD_ARGUMENT(argument2), true) {}

        ArgumentBunch(Node *origin, List *list) : GenericList<Argument *>(origin, true) {
            if(list) {
                List::Iterator i(list);
                while(Node *node = i.next()) {
                    Primitive *primitive = Primitive::dynamicCast(node);
                    if(!primitive) CHILD_THROW(ArgumentException, "Primitive expected");
                    append(primitive); // FIXME: multiple hasChanged() triggered
                }
            }
        }

        CHILD_DECLARE_AND_DEFINE_COPY_METHOD(ArgumentBunch);
        CHILD_DECLARE_AND_DEFINE_FORK_METHOD(ArgumentBunch);

        using GenericList<Argument *>::append;
        void append(Primitive *value);
        void append(Primitive *label, Primitive *value);
        void append(const Bunch *bunch);

        void checkSize(short min, short max = -1) { checkSpecifiedSize(size(), min, max); }
        static void checkSpecifiedSize(short size, short min, short max = -1);

        virtual QString toString(bool debug = false, short level = 0) const {
            return join(", ", "", "", debug, level);
        }
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_ARGUMENT_BUNCH_H
