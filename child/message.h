#ifndef CHILD_MESSAGE_H
#define CHILD_MESSAGE_H

#include "child/block.h"
#include "child/language/argument.h"

CHILD_BEGIN

using namespace Language;

#define CHILD_MESSAGE(ARGS...) \
(new Message(Node::context()->child("Object", "Message"), ##ARGS))

class Message : public Object {
    CHILD_DECLARE(Message, Object);
public:
    explicit Message(const Node *origin, const QString &name = "") :
        Object(origin), _name(name), _inputs(NULL), _outputs(NULL), _block(NULL) {}

    // TODO: use implicit conversion to limit the number of constructors
    Message(const Node *origin, const QString &name, ArgumentBunch *inputs) :
        Object(origin), _name(name), _inputs(inputs), _outputs(NULL), _block(NULL) {}

    Message(const Node *origin, const QString &name, ArgumentBunch *inputs, ArgumentBunch *outputs) :
        Object(origin), _name(name), _inputs(inputs), _outputs(outputs), _block(NULL) {}

    Message(const Node *origin, const QString &name, ArgumentBunch *inputs, ArgumentBunch *outputs, Block *block) :
        Object(origin), _name(name), _inputs(inputs), _outputs(outputs), _block(block) {}

    Message(const Node *origin, const QString &name, Argument *input) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input)), _outputs(NULL), _block(NULL) {}

    Message(const Node *origin, const QString &name, Argument *input1, Argument *input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input1, input2)), _outputs(NULL), _block(NULL) {}

    Message(const Node *origin, const QString &name, Node *input) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input)), _outputs(NULL), _block(NULL) {}

    Message(const Node *origin, const QString &name, Argument *input1, Node *input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input1, CHILD_ARGUMENT(input2))), _outputs(NULL), _block(NULL) {}

    Message(const Node *origin, const QString &name, Node *input1, Argument *input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(CHILD_ARGUMENT(input1), input2)), _outputs(NULL), _block(NULL) {}

    Message(const Node *origin, const QString &name, Node *input1, Node *input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input1, input2)), _outputs(NULL), _block(NULL) {}

    static void initRoot() { Object::root()->addChild("Message", root()); }

    virtual Message *fork() const {
        Message *message = new Message(this);
        if(!name().isEmpty()) message->setName(name());
        if(inputs(false)) message->setInputs(ArgumentBunch::cast(inputs()->fork()));
        if(outputs(false)) message->setOutputs(ArgumentBunch::cast(outputs()->fork()));
        if(block()) message->setBlock(Block::cast(block()->fork()));
        return message;
    }

    const QString name() const { return _name; }
    void setName(const QString &name) { _name = name; }

    ArgumentBunch *inputs(bool createIfNull = true) const {
        if(!_inputs && createIfNull) const_cast<Message *>(this)->_inputs = CHILD_ARGUMENT_BUNCH();
        return(_inputs);
    }

    void setInputs(ArgumentBunch *inputs) { _inputs = inputs; }

    Argument *input(short i) const { return inputs(false)->get(i); }
    bool hasInput(short i) const { return inputs(false) && inputs()->hasIndex(i); }
    Node *runInput(short i, Node *receiver = context()) const { return input(i)->run(receiver); }

    Argument *firstInput() const { return inputs(false)->first(); }
    Node *runFirstInput(Node *receiver = context()) const { return firstInput()->run(receiver); }

    Argument *secondInput() const { return inputs(false)->second(); }
    Node *runSecondInput(Node *receiver = context()) const { return secondInput()->run(receiver); }

    Argument *thirdInput() const { return inputs(false)->third(); }
    Node *runThirdInput(Node *receiver = context()) const { return thirdInput()->run(receiver); }

    ArgumentBunch *outputs(bool createIfNull = true) const {
        if(!_outputs && createIfNull) const_cast<Message *>(this)->_outputs = CHILD_ARGUMENT_BUNCH();
        return(_outputs);
    }

    void setOutputs(ArgumentBunch *outputs) { _outputs = outputs; }

    Block *block() const { return _block; }
    void setBlock(Block *block) { _block = block; }
    bool hasBlock() const { return block(); }

    virtual Node *run(Node *receiver = context()) {
        return receiver->child(name())->run(receiver, this);
    }

    Node *runInputOrSection(const short inputIndex, const QString &sectionLabel,
                              Node *defaultResult = NULL,
                              Node *receiver = context()) const {
        Node *code = hasInputOrSection(inputIndex, sectionLabel);
        return code ? code->run(receiver) : defaultResult;
    }

    Node *hasInputOrSection(const short inputIndex, const QString &sectionLabel) const {
        Section *section = block() ? block()->section(sectionLabel) : NULL;
        if(hasInput(inputIndex)) {
            if(section)
                CHILD_THROW(ArgumentException, "ambiguous alternative forms, cannot choose between \"code passed by argument\""
                            " and \"code passed by block\"");
            return input(inputIndex);
        } else if(section)
            return section;
        else
            return NULL;
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
    ArgumentBunch *_inputs;
    ArgumentBunch *_outputs;
    Block *_block;
};

CHILD_END

#endif // CHILD_MESSAGE_H
