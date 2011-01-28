#include "node/object/text.h"
#include "node/object/language/interpreter.h"

CHILD_BEGIN

// === Text ===

CHILD_DEFINE(Text, Element, Object);

Text::Text(Node *origin, const QString &value, bool isTranslatable, QList<IntPair> *interpolableSlices) :
    GenericElement<QString>(origin, ""), _isTranslatable(isTranslatable) {
    setValue(value); setInterpolableSlices(interpolableSlices);
}

Text::Text(const Text &other) : GenericElement<QString>(other), _isTranslatable(other.isTranslatable()) {
    setValue(other.value());
    setInterpolableSlices(other.interpolableSlices());
}

void Text::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Text, init);

    CHILD_ADD_NATIVE_METHOD(Text, concatenate, +);
    CHILD_ADD_NATIVE_METHOD(Text, multiply, *);

    CHILD_ADD_NATIVE_METHOD(Text, uppercase);
    CHILD_ADD_NATIVE_METHOD(Text, lowercase);
    CHILD_ADD_NATIVE_METHOD(Text, capitalize);

    CHILD_ADD_NATIVE_METHOD(Text, size);
    CHILD_ADD_NATIVE_METHOD(Text, empty);

    CHILD_ADD_NATIVE_METHOD(Text, equal_to, ==);
    CHILD_ADD_NATIVE_METHOD(Text, compare, <=>);
}

CHILD_DEFINE_NATIVE_METHOD(Text, init) {
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

Node *Text::run(Node *receiver) {
    Q_UNUSED(receiver);
    if(interpolableSlices()) {
        CHILD_PUSH_RUN(this);
        QString result = value();
        QString source;
        QString str;
        int offset = 0;
        foreach (IntPair slice, *interpolableSlices()) {
            source = result.mid(offset + slice.first + 1, slice.second - 2);
            str = !source.isEmpty() ? Interpreter::root()->runSourceCode("child:" + source)->toString() : "";
            result.replace(offset + slice.first, slice.second, str);
            offset += str.size() - slice.second;
        }
        return CHILD_TEXT(result);
    } else
        return this;
}

CHILD_DEFINE_NATIVE_METHOD(Text, concatenate) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_TEXT(value() + message->runFirstInput()->toString());
}

CHILD_DEFINE_NATIVE_METHOD(Text, multiply) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_TEXT(value().repeated(message->runFirstInput()->toDouble()));
}

CHILD_DEFINE_NATIVE_METHOD(Text, uppercase) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    if(!message->isExclaimed())
        return CHILD_TEXT(value().toUpper());
    else {
        setValue(value().toUpper());
        return this;
    }
}

CHILD_DEFINE_NATIVE_METHOD(Text, lowercase) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    if(!message->isExclaimed())
        return CHILD_TEXT(value().toLower());
    else {
        setValue(value().toLower());
        return this;
    }
}

QString Text::capitalize(QString text) {
    if(!text.isEmpty()) text[0] = text[0].toUpper();
    return text;
}

CHILD_DEFINE_NATIVE_METHOD(Text, capitalize) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    if(!message->isExclaimed())
        return CHILD_TEXT(capitalize(value()));
    else {
        setValue(capitalize(value()));
        return this;
    }
}

CHILD_DEFINE_NATIVE_METHOD(Text, size) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_NUMBER(value().size());
}

CHILD_DEFINE_NATIVE_METHOD(Text, empty) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_QUESTION_MARK;
    CHILD_CHECK_INPUT_SIZE(0);
    return CHILD_BOOLEAN(value().isEmpty());
}

bool Text::isEqualTo(const Node *other) const {
    const Text *otherText = Text::dynamicCast(other);
    return otherText && value() == otherText->value();
}

CHILD_DEFINE_NATIVE_METHOD(Text, equal_to) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_BOOLEAN(value() == message->runFirstInput()->toString());
}

short Text::compare(const Node *other) const {
    return compare(Text::cast(other)->value());
}

short Text::compare(const QString &other) const {
    int result = value().compare(other);
    if(result > 0) return 1;
    else if(result < 0) return -1;
    else return 0;
}

CHILD_DEFINE_NATIVE_METHOD(Text, compare) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(1);
    return CHILD_NUMBER(compare(message->runFirstInput()->toString()));
}

QString Text::unescapeSequence(const QString &source, QList<IntPair> *interpolableSlices) {
    QString result;
    int i = 0;
    QChar c;
    while(i < source.size()) {
        c = source.at(i);
        if(c == '{' && interpolableSlices) {
            int end = source.indexOf('}', i + 1);
            if(end == -1) CHILD_THROW(RuntimeException, "'}' not found after '{'");
            interpolableSlices->append(IntPair(result.size(), end - i + 1));
            result += source.mid(i, end - i + 1);
            i = end + 1;
        } else {
            if(c == '\\') {
                i++;
                if(i == source.size()) CHILD_THROW(RuntimeException, "invalid escape sequence: '\\'");
                c = source.at(i);
                switch(c.toAscii()) {
                case 't': c = '\t';  break;
                case 'n': c = '\n'; break;
                case 'r': c = '\r'; break;
                case '"': c = '"'; break;
                case '\'': c = '\''; break;
                case '{': c = '{'; break;
                case '\\': c = '\\'; break;
                default:
                    if(QString("01234567xu").contains(c, Qt::CaseInsensitive))
                        c = Text::unescapeSequenceNumber(source, i);
                    else
                        CHILD_THROW(RuntimeException, QString("unknown escape sequence: '\\%1'").arg(c));
                }
            }
            result += c;
            i++;
        }
    }
    return result;
}

QChar Text::unescapeSequenceNumber(const QString &source, int &i) {
    char type;
    QString allowedChars;
    short maxSize;
    QChar c = source.at(i);
    if(c == 'x' || c == 'X') {
        type = 'x';
        allowedChars = "0123456789abcdef";
        maxSize = 2;
    } else if(c == 'u' || c == 'U') {
        type = 'u';
        allowedChars = "0123456789abcdef";
        maxSize = 4;
    } else {
        type = 'o';
        allowedChars = "01234567";
        maxSize = 3;
        i--;
    }
    QString number = "";
    do {
        i++;
        if(i == source.size()) CHILD_THROW(RuntimeException, "invalid escape sequence number");
        c = source.at(i);
        if(!allowedChars.contains(c, Qt::CaseInsensitive)) { i--; break; }
        number += c;
    } while(number.size() < maxSize);
    if(number.isEmpty()) CHILD_THROW(RuntimeException, "invalid escape sequence number");
    bool ok;
    ushort code = type == 'o' ? number.toUShort(&ok, 8) : number.toUShort(&ok, 16);
    if(!ok || (type != 'u' && code > 0xFF)) CHILD_THROW(RuntimeException, "invalid number in escape sequence");
    return QChar(code);
}

double Text::toDouble(bool *okPtr) const {
    bool ok;
    double number = value().toDouble(&ok);
    if(okPtr)
        *okPtr = ok;
    else if(!ok)
        CHILD_THROW_CONVERSION_EXCEPTION("conversion from Text to Number failed");
    return ok ? number : 0;
};

QChar Text::toChar() const {
    if(value().size() != 1) CHILD_THROW_CONVERSION_EXCEPTION("conversion from Text to Character failed (size should be equal to 1)");
    return value().at(0);
};

QString Text::toString(bool debug, short level) const {
    Q_UNUSED(level);
    return debug ? "\"" + value() + "\"" : value();
}

// === Text::Iterator ===

CHILD_DEFINE(Text::Iterator, Object, Text);

void Text::Iterator::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Text::Iterator, init);

    CHILD_ADD_NATIVE_METHOD(Text::Iterator, value);

    CHILD_ADD_NATIVE_METHOD(Text::Iterator, first);
    CHILD_ADD_NATIVE_METHOD(Text::Iterator, last);

    CHILD_ADD_NATIVE_METHOD(Text::Iterator, previous);
    CHILD_ADD_NATIVE_METHOD(Text::Iterator, next);
}

CHILD_DEFINE_NATIVE_METHOD(Text::Iterator, init) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    _text = Text::dynamicCast(origin()->parent());
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Text::Iterator, value) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    if(!message->isQuestioned())
        return CHILD_TEXT(value());
    else
        return CHILD_BOOLEAN(hasValue());
}

CHILD_DEFINE_NATIVE_METHOD(Text::Iterator, first) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    first();
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Text::Iterator, last) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    last();
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Text::Iterator, previous) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    previous();
    return this;
}

CHILD_DEFINE_NATIVE_METHOD(Text::Iterator, next) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    next();
    return this;
}

CHILD_END
