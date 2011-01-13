#include "node/message.h"
#include "node/language/argument.h"

CHILD_BEGIN

namespace Language {
    CHILD_DEFINE(Argument, Pair);

    void Argument::initRoot() {
        Language::root()->addChild("Argument", this);
    }

    QString Argument::labelName() const {
        if(label()->hasNext()) CHILD_THROW(ArgumentException, "illegal parameter label");
        Message *labelMessage = Message::dynamicCast(label()->value());
        if(!labelMessage) CHILD_THROW(ArgumentException, "illegal parameter label");
        return labelMessage->name();
    }

    QString Argument::toString(bool debug, short level) const {
        return concatenateStrings(label() ? label()->toString(debug, level) + ":" : "",
                                  " ",
                                  value() ? value()->toString(debug, level) : "");
    }
}

CHILD_END
