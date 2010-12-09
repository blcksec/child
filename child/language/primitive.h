#ifndef CHILD_LANGUAGE_PRIMITIVE_H
#define CHILD_LANGUAGE_PRIMITIVE_H

#include "child/element.h"
#include "child/language.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Primitive, Element);

    #define CHILD_PRIMITIVE(ARGS...) \
    Language::PrimitivePtr(new Language::Primitive(Node::context()->child("Object", "Language", "Primitive"), ##ARGS))

    class Primitive : public Element {
        CHILD_DECLARATION(Primitive, Element);
    public:
        Primitive(const NodePtr &origin, const NodePtr &value = NULL, const QStringRef &sourceCodeRef = NULL) :
            Element(origin, value), _sourceCodeRef(sourceCodeRef) {}

        static void initRoot() { Language::root()->addChild("Primitive", root()); }
        virtual NodePtr fork() const { return new Primitive(this, value(), sourceCodeRef()); }

        const QStringRef &sourceCodeRef() const { return _sourceCodeRef; }
        void setSourceCodeRef(const QStringRef &sourceCodeRef) { _sourceCodeRef = sourceCodeRef; }
    private:
        QStringRef _sourceCodeRef;
    };

    CHILD_PTR_DEFINITION(Primitive, Element);
}

CHILD_END

#endif // CHILD_LANGUAGE_PRIMITIVE_H
