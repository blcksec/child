#ifndef STRING_H
#define STRING_H

#include "child/nativemethod.h"
#include "child/object.h"

#define CHILD_TEXT(EXPRESSION) static_cast<Text *>(EXPRESSION)

namespace Child {
    class Text : public Object {
    public:
        static Text *root();

        virtual Text *fork() { return(_fork(this)->setValue(_value)); }
        virtual Text *fork(const QString &value) { return(_fork(this)->setValue(value)); }

        const QString value() const { return(_value); }
        Text *setValue(const QString &value) { _value = value; return(this); }

        Node *upcase() { return(CHILD_TEXT(child("Text"))->fork(value().toUpper())); }
    private:
        static Text *_root;
        QString _value;
    };
}

#endif // STRING_H
