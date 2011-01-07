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
        explicit Parameter(Node *origin, const QString &label = NULL, Primitive *defaultValue = NULL,
                           bool isEscaped = false, bool isParented = false) :
            GenericPair<QString, Primitive *>(origin, label, defaultValue),
            _isEscaped(isEscaped), _isParented(isParented) {}

        CHILD_FORK_METHOD(Parameter, label(), CHILD_FORK_IF_NOT_NULL(defaultValue()), isEscaped(), isParented());

        // aliases...
        QString label() const { return key(); }
        void setLabel(const QString &label) { setKey(label); }
        Primitive *defaultValue() const { return value(); }
        void setDefaultValue(Primitive *defaultValue) { setValue(defaultValue); }

        virtual Node *run(Node *receiver = context()) {
            return defaultValue()->run(receiver);
        }

        bool isEscaped() const { return _isEscaped; }
        void setIsEscaped(bool isEscaped) { _isEscaped = isEscaped; }

        bool isParented() const { return _isParented; }
        void setIsParented(bool isParented) { _isParented = isParented; }

        virtual QString toString(bool debug = false, short level = 0) const;
    private:
        bool _isEscaped;
        bool _isParented;
    };

    // === ParameterList ===

    #define CHILD_PARAMETER_LIST(ARGS...) \
    new Language::ParameterList(Node::context()->child("Object", "Language", "ParameterList"), ##ARGS)

    class ParameterList : public GenericList<Parameter *> {
        CHILD_DECLARE(ParameterList, List);
    public:
        explicit ParameterList(Node *origin) : GenericList<Parameter *>(origin) {}

        CHILD_FORK_METHOD(ParameterList);

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

#endif // CHILD_LANGUAGE_PARAMETER_H
