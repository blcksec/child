#ifndef CHILD_TEXT_H
#define CHILD_TEXT_H

#include "child/element.h"
#include "child/number.h"
#include "child/language/argument.h"

CHILD_BEGIN

CHILD_POINTER_DECLARE(Text, Element);

#define CHILD_TEXT(ARGS...) TextPointer(new Text(Node::context()->child("Object", "Text"), ##ARGS))

class Text : public GenericElement<TextPointer, QString> {
    CHILD_DECLARE(Text, Element);
public:
    Text(const Pointer &origin, const QString &value = "") : GenericElement<TextPointer, QString>(origin, value) {}

    static void initRoot() {
        Object::root()->addChild("Text", root());
        CHILD_NATIVE_METHOD_ADD(Text, concatenate, +);
        CHILD_NATIVE_METHOD_ADD(Text, multiply, *);
        CHILD_NATIVE_METHOD_ADD(Text, upcase);
    }
    virtual Pointer fork() const { return new Text(this, value()); }

    CHILD_NATIVE_METHOD_DECLARE(concatenate) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_TEXT(value() + inputs->first()->run()->toString());
    }

    CHILD_NATIVE_METHOD_DECLARE(multiply) {
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_TEXT(value().repeated(inputs->first()->run()->toDouble()));
    }

    CHILD_NATIVE_METHOD_DECLARE(upcase) {
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_TEXT(value().toUpper());
    }

    virtual short compare(const Node &other) const {
        if(this == &other) return 0;
        TextPointer otherText(other, true);
        if(!otherText) return 1;
        int result = value().compare(otherText->value());
        if(result > 0) return 1;
        else if(result < 0) return -1;
        else return 0;
    }

    static QString unescapeSequence(const QString &source);
    static QChar unescapeSequenceNumber(const QString &source, int &i);

    virtual const double toDouble() const {
        bool ok;
        double number = value().toDouble(&ok);
        if(!ok) CHILD_THROW_CONVERSION_EXCEPTION(QString("conversion to Number failed"));
        return number;
    };

    virtual const QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(level);
        return debug ? "\"" + value() + "\"" : value();
    }
};

CHILD_POINTER_DEFINE(Text, Element);

CHILD_END

#endif // CHILD_TEXT_H
