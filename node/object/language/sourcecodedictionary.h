#ifndef CHILD_LANGUAGE_SOURCE_CODE_DICTIONARY_H
#define CHILD_LANGUAGE_SOURCE_CODE_DICTIONARY_H

#include "node/object/dictionary.h"
#include "node/object/language/sourcecode.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_SOURCE_CODE_DICTIONARY(ARGS...) \
    new Language::SourceCodeDictionary(context()->child("Object", "Language", "SourceCodeDictionary"), ##ARGS)

    class SourceCodeDictionary : public GenericDictionary<Node::Reference, SourceCode *> {
        CHILD_DECLARE(SourceCodeDictionary, Dictionary, Language);
    public:
        SourceCodeDictionary(Node *origin) :
            GenericDictionary<Node::Reference, SourceCode *>(origin) {}

        CHILD_DECLARE_AND_DEFINE_COPY_METHOD(SourceCodeDictionary);
        CHILD_DECLARE_AND_DEFINE_FORK_METHOD(SourceCodeDictionary);
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_SOURCE_CODE_DICTIONARY_H
