#ifndef CHILD_LANGUAGE_PRIMITIVE_H
#define CHILD_LANGUAGE_PRIMITIVE_H

#include "child/element.h"
#include "child/language.h"

CHILD_BEGIN

namespace Language {
    CHILD_POINTER_DECLARATION(Primitive, Element);

    #define CHILD_PRIMITIVE(ARGS...) \
    Language::PrimitivePointer(new Language::Primitive(Node::context()->child("Object", "Language", "Primitive"), ##ARGS))

    class Primitive : public Element {
        CHILD_DECLARATION(Primitive, Element);
    public:
        Primitive(const Pointer &origin, const Pointer &value = NULL, const QStringRef &sourceCodeRef = QStringRef()) :
            Element(origin, value), _sourceCodeRef(sourceCodeRef) {}

        static void initRoot() { Language::root()->addChild("Primitive", root()); }
        virtual Pointer fork() const { return new Primitive(this, forkIfNotNull(value()), sourceCodeRef()); }

        const QStringRef &sourceCodeRef() const { return _sourceCodeRef; }
        void setSourceCodeRef(const QStringRef &sourceCodeRef) { _sourceCodeRef = sourceCodeRef; }

        virtual Pointer run(const Pointer &receiver = context()) {
            try {
                return value()->run(receiver);
            } catch(ExceptionPointer e) {
                if(!sourceCodeRef().isNull()) {
                    const QString &source = *(sourceCodeRef().string());
                    int column, line;
                    computeColumnAndLineForPosition(source, sourceCodeRef().position(), column, line);
                    QString text = extractLine(source, line);
                    if(!text.isEmpty()) {
                        QString cursor = QString(" ").repeated(column - 1).append("^");
                        e->message += "\n" + text + "\n" + cursor;
                        e->line = line;
                        e->function.clear();
                    }
                }
                throw;
            }
        }
    private:
        QStringRef _sourceCodeRef;
    };

    CHILD_POINTER_DEFINITION(Primitive, Element);
}

CHILD_END

#endif // CHILD_LANGUAGE_PRIMITIVE_H
