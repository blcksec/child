#ifndef CHILD_LANGUAGE_SOURCE_CODE_DICTIONARY_H
#define CHILD_LANGUAGE_SOURCE_CODE_DICTIONARY_H

#include "child/dictionary.h"
#include "child/language/sourcecode.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_SOURCE_CODE_DICTIONARY(ARGS...) \
    new Language::SourceCodeDictionary(Node::context()->child("Object", "Language", "SourceCodeDictionary"), ##ARGS)

    class SourceCodeDictionary : public GenericDictionary<Node::Reference, SourceCode *> {
        CHILD_DECLARE(SourceCodeDictionary, Dictionary);
    public:
        SourceCodeDictionary(Node *origin) :
            GenericDictionary<Node::Reference, SourceCode *>(origin) {}

        CHILD_FORK_METHOD(SourceCodeDictionary);
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_SOURCE_CODE_DICTIONARY_H
