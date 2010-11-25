#ifndef CHILD_TEXT_H
#define CHILD_TEXT_H

#include "child/nativemethod.h"
#include "child/object.h"

namespace Child {
    class Text : public Object {
        CHILD_DECLARATION(Text, Object, Object);
    public:
        static Text *fork(Node *world, const QString &value) {
            return Text::fork(world)->setValue(value);
        }

        virtual void initFork() { setValue(Text::as(origin())->_value); }

        const QString value() const { return _value; }
        Text *setValue(const QString &value) { _value = value; return this; }

        Node *upcase() { return Text::fork(this, value().toUpper()); }

        virtual Comparison compare(const Node *other) const {
            const Text *otherText = Text::is(other);
            if(!otherText) return(Different);
            int result = value().compare(otherText->value());
            if(result > 0) return Greater;
            else if(result < 0) return Smaller;
            else return Equal;
        }

        virtual uint hash() const { return qHash(value()); }

        static QString unescapeSequence(const QString &source);
        static QChar unescapeSequenceNumber(const QString &source, int &i);

        virtual const QString inspect() const { return QString("\"%1\"").arg(value()); }
    private:
        QString _value;
    };
}

#endif // CHILD_TEXT_H
