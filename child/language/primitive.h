#ifndef CHILD_LANGUAGE_PRIMITIVE_H
#define CHILD_LANGUAGE_PRIMITIVE_H

#include "child/element.h"
#include "child/language.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_PRIMITIVE(ARGS...) \
    new Language::Primitive(Node::context()->child("Object", "Language", "Primitive"), ##ARGS)

    class Primitive : public Element {
        CHILD_DECLARE(Primitive, Element);
    public:
        explicit Primitive(const Node *origin, Node *value = NULL, const QStringRef &sourceCodeRef = QStringRef()) :
            Element(origin, value), _sourceCodeRef(sourceCodeRef) {}

        static void initRoot() { Language::root()->addChild("Primitive", root()); }
        virtual Primitive *fork() const { return new Primitive(this, CHILD_FORK_IF_NOT_NULL(value()), sourceCodeRef()); }

        const QStringRef &sourceCodeRef() const { return _sourceCodeRef; }
        void setSourceCodeRef(const QStringRef &sourceCodeRef) { _sourceCodeRef = sourceCodeRef; }

        virtual Node *run(Node *receiver = context()) {
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
}

CHILD_END

#endif // CHILD_LANGUAGE_PRIMITIVE_H
