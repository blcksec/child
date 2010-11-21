#ifndef CHILD_PRIMITIVE_H
#define CHILD_PRIMITIVE_H

#include "child/object.h"

#define CHILD_PRIMITIVE(EXPRESSION) static_cast<Primitive *>(EXPRESSION)

namespace Child {
    class Primitive : public Object {
    public:
        static Primitive *root();
        static Primitive *fork(Node *world) { return(CHILD_PRIMITIVE(world->child("Primitive"))->fork()); }

        Primitive() : _value(NULL) {}

        virtual ~Primitive() {
            delete _value;
        }

        Node *value() const { return(_value); }
        void setValue(Node *value) { _value = value; }

        const QStringRef &sourceCodeRef() const { return(_sourceCodeRef); }
        void setSourceCodeRef(const QStringRef &sourceCodeRef) { _sourceCodeRef = sourceCodeRef; }

        virtual Primitive *fork() { return(_fork(this)); }

        virtual const QString inspect() const {
            if(!value()) return("NULL");
            return(value()->inspect());
        }
    private:
        static Primitive *_root;
        Node *_value;
        QStringRef _sourceCodeRef;
    };
}

#endif // CHILD_PRIMITIVE_H
