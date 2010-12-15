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
    Message(const Pointer &origin, const QString &name = "") : Object(origin), _name(name) {}

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

    ArgumentBunchPointer outputs(bool createIfNull = true) const {
        if(!_outputs && createIfNull) const_cast<Message *>(this)->_outputs = CHILD_ARGUMENT_BUNCH();
        return(_outputs);
    }

    void setOutputs(const ArgumentBunchPointer &outputs) { _outputs = outputs; }

    BlockPointer block() const { return _block; }
    void setBlock(const BlockPointer &block) { _block = block; }

    virtual Pointer run(const Pointer &receiver = context()) {
        Pointer result = receiver->child(name());
        NativeMethodPointer nativeMethod(result, true);
        if(nativeMethod) result = ((*const_cast<Node *>(receiver.data())).*nativeMethod->method())(inputs(false));
        return result;
    }

    virtual const QString toString(bool debug = false, short level = 0) const {
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
