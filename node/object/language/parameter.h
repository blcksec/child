#ifndef CHILD_LANGUAGE_PARAMETER_H
#define CHILD_LANGUAGE_PARAMETER_H

#include "node/object/pair.h"
#include "node/object/language/primitive.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_PARAMETER(ARGS...) \
    new Language::Parameter(context()->child("Object", "Language", "Parameter"), ##ARGS)

    class Parameter : public GenericPair<QString, Primitive *> {
        CHILD_DECLARE(Parameter, Pair, Language);
    public:
        explicit Parameter(Node *origin, const QString &label = NULL, Primitive *defaultValue = NULL,
                           bool isEscaped = false, bool isParented = false) :
            GenericPair<QString, Primitive *>(origin, label, defaultValue),
            _isEscaped(isEscaped), _isParented(isParented) {}

        CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Parameter, label(), CHILD_FORK_IF_NOT_NULL(defaultValue()), isEscaped(), isParented());

        // aliases...
        QString label() const { return key(); }
        void setLabel(const QString &label) { setKey(label); }
        Primitive *defaultValue() const { return value(); }
        void setDefaultValue(Primitive *defaultValue) { setValue(defaultValue); }

        virtual Node *run(Node *receiver = context()) {
            // CHILD_PUSH_RUN(this); // is it really necessary?
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
}

CHILD_END

#endif // CHILD_LANGUAGE_PARAMETER_H
