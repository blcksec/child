#ifndef CHILD_TEXT_H
#define CHILD_TEXT_H

#include "node/object/element.h"
#include "node/object/boolean.h"
#include "node/object/number.h"
#include "node/object/message.h"

CHILD_BEGIN

#define CHILD_TEXT(ARGS...) new Text(context()->child("Object", "Text"), ##ARGS)

class Text : public GenericElement<QString> {
    CHILD_DECLARE(Text, Element, Object);
public:
    explicit Text(Node *origin, const QString &value = "", bool isTranslatable = false,
                  QList<IntPair> *interpolableSlices = NULL) : GenericElement<QString>(origin, ""),
    _isTranslatable(isTranslatable) { setValue(value); setInterpolableSlices(interpolableSlices); }

    Text(const Text &other) : GenericElement<QString>(other), _isTranslatable(other.isTranslatable()) {
        setValue(other.value());
        setInterpolableSlices(other.interpolableSlices());
    }

    virtual ~Text() {
        delete _interpolableSlices;
    }

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Text);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Text, value(), isTranslatable(), interpolableSlices());

    CHILD_DECLARE_NATIVE_METHOD(init) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0, 1);
        if(message->hasInput(0)) setValue(message->runFirstInput()->toString());

        // === TODO: DRY ===
        CHILD_FIND_LAST_PRIMITIVE;
        Primitive *nextPrimitive = primitive->next();
        if(nextPrimitive) {
            nextPrimitive->run(this);
            Primitive::skip(this);
        }

        return this;
    }

    bool isTranslatable() const { return _isTranslatable; }
    void setIsTranslatable(bool isTranslatable) { _isTranslatable = isTranslatable; }

    QList<IntPair> *interpolableSlices() const { return _interpolableSlices; }

    void setInterpolableSlices(QList<IntPair> *interpolableSlices) {
        _interpolableSlices = interpolableSlices && !interpolableSlices->isEmpty() ?
                    new QList<IntPair>(*interpolableSlices) : NULL;
    }

    virtual Node *run(Node *receiver = context());

    CHILD_DECLARE_NATIVE_METHOD(concatenate) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_TEXT(value() + message->runFirstInput()->toString());
    }

    CHILD_DECLARE_NATIVE_METHOD(multiply) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_TEXT(value().repeated(message->runFirstInput()->toDouble()));
    }

    CHILD_DECLARE_NATIVE_METHOD(uppercase) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        if(!message->isExclaimed())
            return CHILD_TEXT(value().toUpper());
        else {
            setValue(value().toUpper());
            return this;
        }
    }

    CHILD_DECLARE_NATIVE_METHOD(lowercase) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        if(!message->isExclaimed())
            return CHILD_TEXT(value().toLower());
        else {
            setValue(value().toLower());
            return this;
        }
    }

    static QString capitalize(QString text) {
        if(!text.isEmpty()) text[0] = text[0].toUpper();
        return text;
    }

    CHILD_DECLARE_NATIVE_METHOD(capitalize) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        if(!message->isExclaimed())
            return CHILD_TEXT(capitalize(value()));
        else {
            setValue(capitalize(value()));
            return this;
        }
    }

    CHILD_DECLARE_NATIVE_METHOD(size) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_NUMBER(value().size());
    }

    CHILD_DECLARE_NATIVE_METHOD(empty) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_QUESTION_MARK;
        CHILD_CHECK_INPUT_SIZE(0);
        return CHILD_BOOLEAN(value().isEmpty());
    }

    virtual bool isEqualTo(const Node *other) const {
        const Text *otherText = Text::dynamicCast(other);
        return otherText && value() == otherText->value();
    }

    CHILD_DECLARE_NATIVE_METHOD(equal_to) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_BOOLEAN(value() == message->runFirstInput()->toString());
    }

    virtual short compare(const Node *other) const {
        return compare(Text::cast(other)->value());
    }

    short compare(const QString &other) const {
        int result = value().compare(other);
        if(result > 0) return 1;
        else if(result < 0) return -1;
        else return 0;
    }

    CHILD_DECLARE_NATIVE_METHOD(compare) {
        CHILD_FIND_LAST_MESSAGE;
        CHILD_CHECK_INPUT_SIZE(1);
        return CHILD_NUMBER(compare(message->runFirstInput()->toString()));
    }

    static QString unescapeSequence(const QString &source, QList<IntPair> *interpolableSlices = NULL);
    static QChar unescapeSequenceNumber(const QString &source, int &i);

    virtual double toDouble(bool *okPtr = NULL) const {
        bool ok;
        double number = value().toDouble(&ok);
        if(okPtr)
            *okPtr = ok;
        else if(!ok)
            CHILD_THROW_CONVERSION_EXCEPTION("conversion from Text to Number failed");
        return ok ? number : 0;
    };

    virtual QChar toChar() const {
        if(value().size() != 1) CHILD_THROW_CONVERSION_EXCEPTION("conversion from Text to Character failed (size should be equal to 1)");
        return value().at(0);
    };

    virtual QString toString(bool debug = false, short level = 0) const {
        Q_UNUSED(level);
        return debug ? "\"" + value() + "\"" : value();
    }
private:
    bool _isTranslatable;
    QList<IntPair> *_interpolableSlices;
};

CHILD_END

#endif // CHILD_TEXT_H
