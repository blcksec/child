#ifndef CHILD_MESSAGE_H
#define CHILD_MESSAGE_H

#include "child/block.h"
#include "child/language/argument.h"

CHILD_BEGIN

using namespace Language;

CHILD_POINTER_DECLARE(Message, Object);

#define CHILD_MESSAGE(ARGS...) \
MessagePointer(new Message(Node::context()->child("Object", "Message"), ##ARGS))

class Message : public Object {
    CHILD_DECLARE(Message, Object);
public:
    explicit Message(const Pointer &origin, const QString &name = "") : Object(origin), _name(name) {}

    // TODO: use implicit conversion to limit the number of constructors
    Message(const Pointer &origin, const QString &name, const ArgumentBunchPointer &inputs) :
        Object(origin), _name(name), _inputs(inputs) {}

    Message(const Pointer &origin, const QString &name, const ArgumentBunchPointer &inputs,
            const ArgumentBunchPointer &outputs) :
        Object(origin), _name(name), _inputs(inputs), _outputs(outputs) {}

    Message(const Pointer &origin, const QString &name, const ArgumentBunchPointer &inputs,
            const ArgumentBunchPointer &outputs, const BlockPointer &block) :
        Object(origin), _name(name), _inputs(inputs), _outputs(outputs), _block(block) {}

    Message(const Pointer &origin, const QString &name, const ArgumentPointer &input) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input)) {}

    Message(const Pointer &origin, const QString &name, const ArgumentPointer &input1, const ArgumentPointer &input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input1, input2)) {}

    Message(const Pointer &origin, const QString &name, const Pointer &input) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input)) {}

    Message(const Pointer &origin, const QString &name, const ArgumentPointer &input1, const Pointer &input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input1, CHILD_ARGUMENT(input2))) {}

    Message(const Pointer &origin, const QString &name, const Pointer &input1, const ArgumentPointer &input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(CHILD_ARGUMENT(input1), input2)) {}

    Message(const Pointer &origin, const QString &name, const Pointer &input1, const Pointer &input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input1, input2)) {}

    static void initRoot() { Object::root()->addChild("Message", root()); }

    virtual Pointer fork() const {
        MessagePointer message = new Message(this);
        if(!name().isEmpty()) message->setName(name());
        if(inputs(false)) message->setInputs(inputs()->fork());
        if(outputs(false)) message->setOutputs(outputs()->fork());
        if(block()) message->setBlock(block()->fork());
        return message;
    }

    const QString name() const { return _name; }
    void setName(const QString &name) { _name = name; }

    ArgumentBunchPointer inputs(bool createIfNull = true) const {
        if(!_inputs && createIfNull) const_cast<Message *>(this)->_inputs = CHILD_ARGUMENT_BUNCH();
        return(_inputs);
    }

    void setInputs(const ArgumentBunchPointer &inputs) { _inputs = inputs; }

    ArgumentPointer input(short i) const { return inputs(false)->get(i); }
    bool hasInput(short i) const { return inputs(false) && inputs()->hasIndex(i); }
    Pointer runInput(short i, const Pointer &receiver = context()) const { return input(i)->run(receiver); }

    ArgumentPointer firstInput() const { return inputs(false)->first(); }
    Pointer runFirstInput(const Pointer &receiver = context()) const { return firstInput()->run(receiver); }

    ArgumentPointer secondInput() const { return inputs(false)->second(); }
    Pointer runSecondInput(const Pointer &receiver = context()) const { return secondInput()->run(receiver); }

    ArgumentPointer thirdInput() const { return inputs(false)->third(); }
    Pointer runThirdInput(const Pointer &receiver = context()) const { return thirdInput()->run(receiver); }

    ArgumentBunchPointer outputs(bool createIfNull = true) const {
        if(!_outputs && createIfNull) const_cast<Message *>(this)->_outputs = CHILD_ARGUMENT_BUNCH();
        return(_outputs);
    }

    void setOutputs(const ArgumentBunchPointer &outputs) { _outputs = outputs; }

    BlockPointer block() const { return _block; }
    void setBlock(const BlockPointer &block) { _block = block; }
    bool hasBlock() const { return block(); }

    virtual Pointer run(const Pointer &receiver = context()) {
        return receiver->child(name())->run(receiver, this);
    }

    Pointer runInputOrSection(const short inputIndex, const QString &sectionLabel,
                              const Pointer &defaultResult = Pointer::null(),
                              const Pointer &receiver = context()) const {
        Pointer code = hasInputOrSection(inputIndex, sectionLabel);
        return code ? code->run(receiver) : defaultResult;
    }

    Pointer hasInputOrSection(const short inputIndex, const QString &sectionLabel) const {
        SectionPointer section = block() ? block()->section(sectionLabel) : SectionPointer::null();
        if(hasInput(inputIndex)) {
            if(section)
                CHILD_THROW(ArgumentException, "ambiguous alternative forms, cannot choose between \"code passed by argument\""
                            " and \"code passed by block\"");
            return input(inputIndex);
        } else if(section)
            return section;
        else
            return Pointer::null();
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        QString str = name();
        if(inputs(false) && inputs()->isNotEmpty())
            str += "(" + inputs()->toString(debug, level) + ")";
        if(outputs(false) && outputs()->isNotEmpty())
            str += " -> " + outputs()->toString(debug, level);
        if(block()) str += " " + block()->toString(debug, level);
        return str;
    }
private:
    QString _name;
    ArgumentBunchPointer _inputs;
    ArgumentBunchPointer _outputs;
    BlockPointer _block;
};

CHILD_POINTER_DEFINE(Message, Object);

CHILD_END

#endif // CHILD_MESSAGE_H
