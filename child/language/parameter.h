#ifndef CHILD_LANGUAGE_PARAMETER_H
#define CHILD_LANGUAGE_PARAMETER_H

#include "child/pair.h"
#include "child/list.h"
#include "child/language/primitivechain.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_PARAMETER(ARGS...) \
    new Language::Parameter(Node::context()->child("Object", "Language", "Parameter"), ##ARGS)

    class Parameter : public GenericPair<QString, PrimitiveChain *> {
        CHILD_DECLARE(Parameter, Pair);
    public:
        explicit Parameter(const Node *origin, const QString &label = NULL,
                  const PrimitiveChain *defaultValue = NULL) :
            GenericPair<QString, PrimitiveChain *>(origin, label, defaultValue) {}

        static void initRoot() { Language::root()->addChild("Parameter", root()); }

        virtual Node *fork() const {
            return new Parameter(this, label(), forkIfNotNull(defaultValue()));
        }

        // aliases...
        QString label() const { return key(); }
        void setLabel(const QString &label) { setKey(label); }
        PrimitiveChain *defaultValue() const { return value(); }
        void setDefaultValue(const PrimitiveChain *defaultValue) { setValue(defaultValue); }

        virtual Node *run(const Node *receiver = context()) {
            return defaultValue()->run(receiver);
        }

        virtual QString toString(bool debug = false, short level = 0) const {
            return concatenateStrings(label(),
                                      ": ",
                                      defaultValue() ? defaultValue()->toString(debug, level) : "");
        }
    };

    CHILD_POINTER_DEFINE(Parameter, Pair);

    // === ParameterList ===

    CHILD_POINTER_DECLARE(ParameterList, List);

    #define CHILD_PARAMETER_LIST(ARGS...) \
    new Language::ParameterList(Node::context()->child("Object", "Language", "ParameterList"), ##ARGS)

    class ParameterList : public GenericList<Parameter *> {
        CHILD_DECLARE(ParameterList, List);
    public:
        explicit ParameterList(const Node *origin) : GenericList<Parameter *>(origin) {}

        static void initRoot() { Language::root()->addChild("ParameterList", root()); }
        virtual Node *fork() const { return (new ParameterList(this))->initFork(); }

        QHash<QString, Parameter *> labels() { return _labels; }
        Parameter *hasLabel(const QString &label) { return _labels.value(label); }

        virtual void hasChanged() {
            _labels.clear();
            Iterator i(this);
            while(Parameter *parameter = i.next()) {
                if(_labels.contains(parameter->label()))
                    CHILD_THROW(DuplicateException, "duplicated label found in parameter list");
                _labels.insert(parameter->label(), parameter);
            }
        }

        virtual QString toString(bool debug = false, short level = 0) const { return join(", ", "", "", debug, level); }
    private:
        QHash<QString, Parameter *> _labels;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_PARAMETER_H
