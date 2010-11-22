#ifndef CHILD_PRIMITIVE_H
#define CHILD_PRIMITIVE_H

#include "child/object.h"

namespace Child {
    class Primitive : public Object {
        CHILD_DECLARATION(Primitive);
    public:
        Primitive() : _value(NULL) {}

        virtual ~Primitive() {
            delete _value;
        }

        Node *value() const { return(_value); }
        void setValue(Node *value) { _value = value; }

        const QStringRef &sourceCodeRef() const { return(_sourceCodeRef); }
        void setSourceCodeRef(const QStringRef &sourceCodeRef) { _sourceCodeRef = sourceCodeRef; }

        virtual const QString inspect() const {
            if(!value()) return("NULL");
            return(value()->inspect());
        }
    private:
        Node *_value;
        QStringRef _sourceCodeRef;
    };
}

#endif // CHILD_PRIMITIVE_H
