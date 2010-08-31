#ifndef STRING_H
#define STRING_H

#include "child/module.h"

namespace Child {
    class Text : public Module {
    public:
        virtual Text *clone() const;

//        Text(Module *const &genre = NULL, Module *const &parent = NULL) : Module(genre, parent) {}

//        static Text *const create(Module *const &world, const QString &value = "");

//        const QString value() const { return(_value); }
//        void setValue(const QString &value) { _value = value; }

//        Module *toText();

//        Module *upcase() { return(Text::create(this, value().toUpper())); }
    private:
        QString _value;
    };
}

#endif // STRING_H
