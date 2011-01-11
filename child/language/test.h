#ifndef CHILD_LANGUAGE_TEST_H
#define CHILD_LANGUAGE_TEST_H

#include "child/element.h"
#include "child/language/section.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_TEST(ARGS...) \
    new Language::Test(Node::context()->child("Object", "Language", "Test"), ##ARGS)

    class Test : public GenericElement<Section *> {
        CHILD_DECLARE(Test, Element);
    public:
        explicit Test(Node *origin, Section *section = NULL, Node *receiver = NULL) :
            GenericElement<Section *>(origin, section), _receiver(receiver) {}

        CHILD_FORK_METHOD(Test, CHILD_FORK_IF_NOT_NULL(section()), CHILD_FORK_IF_NOT_NULL(receiver()));

        // aliases...
        Section *section() const { return value(); }
        void setSection(Section *section) { setValue(section); }

        Node *receiver() const { return _receiver; }
        void setReceiver(Node *receiver) { _receiver = receiver; }

        virtual Node *run(Node *receiver = context()) {
            Q_UNUSED(receiver);
            ContextPusher pusher(this->receiver()); Q_UNUSED(pusher);
            return section()->run();
        }

        virtual QString toString(bool debug = false, short level = 0) const {
            return section() ? section()->toString(debug, level) : "NULL";
        }
    private:
        Node *_receiver;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_TEST_H
