#ifndef CHILD_LANGUAGE_TEST_SUITE_H
#define CHILD_LANGUAGE_TEST_SUITE_H

#include "node/object/list.h"
#include "node/object/language/test.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_TEST_SUITE(ARGS...) \
    new Language::TestSuite(context()->child("Object", "Language", "TestSuite"), ##ARGS)

    class TestSuite : public GenericList<Test *> {
        CHILD_DECLARE(TestSuite, List, Language);
    public:
        explicit TestSuite(Node *origin) : GenericList<Test *>(origin) {}

        CHILD_DECLARE_AND_DEFINE_COPY_METHOD(TestSuite);
        CHILD_DECLARE_AND_DEFINE_FORK_METHOD(TestSuite);

        virtual Node *run(Node *receiver = context());
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_TEST_SUITE_H
