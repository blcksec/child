#ifndef CHILD_PRIMITIVE_H
#define CHILD_PRIMITIVE_H

#include "child/element.h"
#include "child/language.h"

CHILD_BEGIN

namespace Language {
    CHILD_PTR_DECLARATION(Primitive, Element);

    #define CHILD_PRIMITIVE(ARGS...) \
    PrimitivePtr(new Primitive(Node::findInContext("Object")->child("Language")->child("Primitive"), ##ARGS))

    class Primitive : public Element {
        CHILD_DECLARATION(Primitive, Element);
    public:
        Primitive(const NodePtr &origin, const NodePtr &value = NULL) : Element(origin, value) {}

        static void initRoot() { Language::root()->addChild("Primitive", root()); }
        virtual NodePtr fork() const { return new Primitive(this, value()); }

        const QStringRef &sourceCodeRef() const { return _sourceCodeRef; }
        void setSourceCodeRef(const QStringRef &sourceCodeRef) { _sourceCodeRef = sourceCodeRef; }
    private:
        QStringRef _sourceCodeRef;
    };

    CHILD_PTR_DEFINITION(Primitive, Element);
}

CHILD_END

#endif // CHILD_PRIMITIVE_H
