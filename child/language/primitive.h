#ifndef CHILD_LANGUAGE_PRIMITIVE_H
#define CHILD_LANGUAGE_PRIMITIVE_H

#include "child/element.h"
#include "child/language.h"

CHILD_BEGIN

namespace Language {
    CHILD_POINTER_DECLARE(Primitive, Element);

    #define CHILD_PRIMITIVE(ARGS...) \
    Language::PrimitivePointer(new Language::Primitive(Node::context()->child("Object", "Language", "Primitive"), ##ARGS))

    class Primitive : public Element {
        CHILD_DECLARE(Primitive, Element);
    public:
        Primitive(const Pointer &origin, const Pointer &value = NULL, const QStringRef &sourceCodeRef = QStringRef()) :
            Element(origin, value), _sourceCodeRef(sourceCodeRef) {}

        static void initRoot() { Language::root()->addChild("Primitive", root()); }
        virtual Pointer fork() const { return new Primitive(this, forkIfNotNull(value()), sourceCodeRef()); }

        const QStringRef &sourceCodeRef() const { return _sourceCodeRef; }
        void setSourceCodeRef(const QStringRef &sourceCodeRef) { _sourceCodeRef = sourceCodeRef; }

        virtual Pointer run(const Pointer &receiver = context()) {
            #ifdef CHILD_CATCH_EXCEPTIONS
            try {
            #endif
                return value()->run(receiver);
            #ifdef CHILD_CATCH_EXCEPTIONS
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
            #endif
        }
    private:
        QStringRef _sourceCodeRef;
    };

    CHILD_POINTER_DEFINE(Primitive, Element);
}

CHILD_END

#endif // CHILD_LANGUAGE_PRIMITIVE_H
