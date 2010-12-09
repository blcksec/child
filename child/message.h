#ifndef CHILD_MESSAGE_H
#define CHILD_MESSAGE_H

#include "child/block.h"
#include "child/language/argument.h"

CHILD_BEGIN

using namespace Language;

CHILD_PTR_DECLARATION(Message, Object);

#define CHILD_MESSAGE(ARGS...) \
MessagePtr(new Message(Node::context()->child("Object", "Message"), ##ARGS))

class Message : public Object {
    CHILD_DECLARATION(Message, Object);
public:
    Message(const NodePtr &origin, const QString &name = "") : Object(origin), _name(name) {}

    static void initRoot() { Object::root()->addChild("Message", root()); }

    virtual NodePtr fork() const {
        MessagePtr message = new Message(this);
        if(!name().isEmpty()) message->setName(name());
        if(inputs(false)) message->setInputs(inputs()->fork());
        if(outputs(false)) message->setOutputs(outputs()->fork());
        if(block()) message->setBlock(block()->fork());
        return message;
    }

    const QString name() const { return _name; }
    void setName(const QString &name) { _name = name; }

    ArgumentBunchPtr inputs(bool createIfNull = true) const {
        if(!_inputs && createIfNull) const_cast<Message *>(this)->_inputs = CHILD_ARGUMENT_BUNCH();
        return(_inputs);
    }

    void setInputs(const ArgumentBunchPtr &inputs) { _inputs = inputs; }

    ArgumentBunchPtr outputs(bool createIfNull = true) const {
        if(!_outputs && createIfNull) const_cast<Message *>(this)->_outputs = CHILD_ARGUMENT_BUNCH();
        return(_outputs);
    }

    void setOutputs(const ArgumentBunchPtr &outputs) { _outputs = outputs; }

    BlockPtr block() const { return _block; }
    void setBlock(const BlockPtr &block) { _block = block; }

    virtual const QString toString(bool debug = false) const {
        QString str = name();
        if(inputs(false) && inputs()->isNotEmpty())
            str += "(" + inputs()->toString(debug) + ")";
        if(outputs(false) && outputs()->isNotEmpty())
            str += " -> (" + outputs()->toString(debug) + ")";
        if(block())
            str += " {\n" + block()->toString(debug) + "\n}";
        return str;
    }
private:
    QString _name;
    ArgumentBunchPtr _inputs;
    ArgumentBunchPtr _outputs;
    BlockPtr _block;
};

CHILD_PTR_DEFINITION(Message, Object);

CHILD_END

#endif // CHILD_MESSAGE_H
