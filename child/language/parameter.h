#ifndef CHILD_LANGUAGE_PARAMETER_H
#define CHILD_LANGUAGE_PARAMETER_H

#include "child/pair.h"
#include "child/list.h"
#include "child/language/primitive.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_PARAMETER(ARGS...) \
    new Language::Parameter(Node::context()->child("Object", "Language", "Parameter"), ##ARGS)

    class Parameter : public GenericPair<QString, Primitive *> {
        CHILD_DECLARE(Parameter, Pair);
    public:
        explicit Parameter(Node *origin, const QString &label = NULL,
                  Primitive *defaultValue = NULL) :
            GenericPair<QString, Primitive *>(origin, label, defaultValue) {}

        CHILD_FORK_METHOD(Parameter, label(), CHILD_FORK_IF_NOT_NULL(defaultValue()));

        // aliases...
        QString label() const { return key(); }
        void setLabel(const QString &label) { setKey(label); }
        Primitive *defaultValue() const { return value(); }
        void setDefaultValue(Primitive *defaultValue) { setValue(defaultValue); }

        virtual Node *run(Node *receiver = context()) {
            return defaultValue()->run(receiver);
        }

        virtual QString toString(bool debug = false, short level = 0) const {
            return concatenateStrings(label(),
                                      ": ",
                                      defaultValue() ? defaultValue()->toString(debug, level) : "");
        }
    };

    // === ParameterList ===

    #define CHILD_PARAMETER_LIST(ARGS...) \
    new Language::ParameterList(Node::context()->child("Object", "Language", "ParameterList"), ##ARGS)

    class ParameterList : public GenericList<Parameter *> {
        CHILD_DECLARE(ParameterList, List);
    public:
        explicit ParameterList(Node *origin) : GenericList<Parameter *>(origin) {}

        CHILD_FORK_METHOD(ParameterList);

        QHash<QString, Parameter *> labels() { return _labels; }
        Parameter *hasLabel(const QString &label) { return _labels.value(label); }

        virtual void hasChanged();

        virtual QString toString(bool debug = false, short level = 0) const {
            return join(", ", "", "", debug, level);
        }
    private:
        QHash<QString, Parameter *> _labels;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_PARAMETER_H
