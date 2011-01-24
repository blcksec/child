#ifndef CHILD_LANGUAGE_PARAMETER_LIST_H
#define CHILD_LANGUAGE_PARAMETER_LIST_H

#include "node/object/list.h"
#include "node/object/language/parameter.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_PARAMETER_LIST(ARGS...) \
    new Language::ParameterList(context()->child("Object", "Language", "ParameterList"), ##ARGS)

    class ParameterList : public GenericList<Parameter *> {
        CHILD_DECLARE(ParameterList, List, Language);
    public:
        explicit ParameterList(Node *origin) : GenericList<Parameter *>(origin) {}

        CHILD_DECLARE_AND_DEFINE_COPY_METHOD(ParameterList);
        CHILD_DECLARE_AND_DEFINE_FORK_METHOD(ParameterList);

        using GenericList<Parameter *>::get;
        Parameter *get(const QString &label) { return _labels.value(label); }
        bool hasLabel(const QString &label) { return _labels.contains(label); }

        QHash<QString, Parameter *> labels() { return _labels; }

        virtual void hasChanged();

        virtual QString toString(bool debug = false, short level = 0) const {
            return join(", ", "", "", debug, level);
        }
    private:
        QHash<QString, Parameter *> _labels;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_PARAMETER_LIST_H
