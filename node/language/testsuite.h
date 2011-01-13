#ifndef CHILD_LANGUAGE_TEST_SUITE_H
#define CHILD_LANGUAGE_TEST_SUITE_H

#include "child/list.h"
#include "child/language/test.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_TEST_SUITE(ARGS...) \
    new Language::TestSuite(Node::context()->child("Object", "Language", "TestSuite"), ##ARGS)

    class TestSuite : public GenericList<Test *> {
        CHILD_DECLARE(TestSuite, List);
    public:
        explicit TestSuite(Node *origin) : GenericList<Test *>(origin) {}

        CHILD_FORK_METHOD(TestSuite);

        virtual Node *run(Node *receiver = context());
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_TEST_SUITE_H
