#ifndef CHILD_LANGUAGE_PRIMITIVE_H
#define CHILD_LANGUAGE_PRIMITIVE_H

#include "node/object/element.h"
#include "node/object/language.h"

CHILD_BEGIN

namespace Language {
    #define CHILD_PRIMITIVE(ARGS...) \
    new Language::Primitive(context()->child("Object", "Language", "Primitive"), ##ARGS)

    #define CHILD_PRIMITIVE_ADD(BASE, OTHER) \
    if(BASE) BASE->last()->setNext(OTHER); else BASE = (OTHER);

    class Primitive : public Element {
        CHILD_DECLARE(Primitive, Element, Language);
    public:
        explicit Primitive(Node *origin, Node *value = NULL, const QStringRef &sourceCodeRef = QStringRef(),
                           Primitive *next = NULL) :
            Element(origin, value), _sourceCodeRef(sourceCodeRef),  _previous(NULL), _next(NULL) { setNext(next); }

        virtual ~Primitive() {
            setNext(NULL);
        }

        CHILD_FORK_METHOD(Primitive, CHILD_FORK_IF_NOT_NULL(value()), sourceCodeRef(), CHILD_FORK_IF_NOT_NULL(next()));

        const QStringRef &sourceCodeRef() const { return _sourceCodeRef; }
        void setSourceCodeRef(const QStringRef &sourceCodeRef) { _sourceCodeRef = sourceCodeRef; }

        Primitive *next() const { return _next; }
        Primitive *setNext(Primitive *next);
        bool hasNext() const { return _next; }
        Primitive *last();
        int size() const;

        Primitive *previous() const { return _previous; }
        bool hasPrevious() const { return _previous; }

        virtual Node *run(Node *receiver = context()) {
            Node *result = value()->run(receiver);
            return hasNext() ? result->receive(next()) : result;
//            try {
//            } catch(ExceptionPointer e) {
//                if(!sourceCodeRef().isNull()) {
//                    const QString &source = *(sourceCodeRef().string());
//                    int column, line;
//                    computeColumnAndLineForPosition(source, sourceCodeRef().position(), column, line);
//                    QString text = extractLine(source, line);
//                    if(!text.isEmpty()) {
//                        QString cursor = QString(" ").repeated(column - 1).append("^");
//                        e->message += "\n" + text + "\n" + cursor;
//                        e->line = line;
//                        e->function.clear();
//                    }
//                }
//                throw;
//            }
        }

        Node *runExceptLast(Node *receiver = context()) {
            return hasNext() ? next()->runExceptLast(value()->run(receiver)) : receiver;
        }

        virtual QString toString(bool debug = false, short level = 0) const;
    private:
        QStringRef _sourceCodeRef;
        Primitive *_previous;
        Primitive *_next;
    };
}

CHILD_END

#endif // CHILD_LANGUAGE_PRIMITIVE_H
