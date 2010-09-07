#ifndef STRING_H
#define STRING_H

#include "child/object.h"

namespace Child {
    class Text : public Object {
    public:
        static void initialize();

        virtual Text *fork() const { return(_fork(this)->setValue(_value)); }
        Text *fork(const QString &value) { return(_fork(this)->setValue(value)); }

        const QString value() const { return(_value); }
        Text *setValue(const QString &value) { _value = value; return(this); }

//        Module *toText();

//        Module *upcase() { return(Text::create(this, value().toUpper())); }
    private:
        QString _value;
    };
}

#endif // STRING_H
