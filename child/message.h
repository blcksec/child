#ifndef CHILD_MESSAGE_H
#define CHILD_MESSAGE_H

#include "child/language/argument.h"

CHILD_BEGIN

#define CHILD_MESSAGE(ARGS...) \
(new Message(Node::context()->child("Object", "Message"), ##ARGS))

class Message : public Object {
    CHILD_DECLARE(Message, Object);
public:
    explicit Message(Node *origin, const QString &name = "", ArgumentBunch *inputs = NULL, ArgumentBunch *outputs = NULL,
                     bool isEscaped = false, bool isParented = false, bool isVariadic = false, const QString &codeInputName = "") :
        Object(origin), _name(name), _inputs(inputs), _outputs(outputs),
        _isEscaped(isEscaped), _isParented(isParented), _isVariadic(isVariadic),
        _codeInputName(codeInputName) {}

    Message(Node *origin, const QString &name, Argument *input) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input)), _outputs(NULL),
        _isEscaped(false), _isParented(false), _isVariadic(false) {}

    Message(Node *origin, const QString &name, Argument *input1, Argument *input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input1, input2)), _outputs(NULL),
        _isEscaped(false), _isParented(false), _isVariadic(false) {}

    Message(Node *origin, const QString &name, Node *input) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input)), _outputs(NULL),
        _isEscaped(false), _isParented(false), _isVariadic(false) {}

    Message(Node *origin, const QString &name, Argument *input1, Node *input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input1, CHILD_ARGUMENT(input2))), _outputs(NULL),
        _isEscaped(false), _isParented(false), _isVariadic(false) {}

    Message(Node *origin, const QString &name, Node *input1, Argument *input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(CHILD_ARGUMENT(input1), input2)), _outputs(NULL),
        _isEscaped(false), _isParented(false), _isVariadic(false) {}

    Message(Node *origin, const QString &name, Node *input1, Node *input2) :
        Object(origin), _name(name), _inputs(CHILD_ARGUMENT_BUNCH(input1, input2)), _outputs(NULL),
        _isEscaped(false), _isParented(false), _isVariadic(false) {}

    CHILD_FORK_METHOD(Message, name(), CHILD_FORK_IF_NOT_NULL(inputs(false)), CHILD_FORK_IF_NOT_NULL(outputs(false)),
                      isEscaped(), isParented(), isVariadic(), codeInputName());

    const QString &name() const { return _name; }
    void setName(const QString &name) { _name = name; }

    ArgumentBunch *inputs(bool createIfNull = true) const {
        if(!_inputs && createIfNull) constCast(this)->_inputs = CHILD_ARGUMENT_BUNCH();
        return _inputs;
    }

    void setInputs(ArgumentBunch *inputs) { _inputs = inputs; }

    Argument *input(short i) const { return inputs(false)->get(i); }
    bool hasInput(short i) const { return inputs(false) && inputs()->hasIndex(i); }
    Node *runInput(short i, Node *receiver = context()) const { return input(i)->run(receiver); }

    Argument *firstInput() const { return input(0); }
    bool hasAnInput() const { return hasInput(0); }
    Node *runFirstInput(Node *receiver = context()) const { return runInput(0, receiver); }

    Argument *secondInput() const { return input(1); }
    bool hasASecondInput() const { return hasInput(1); }
    Node *runSecondInput(Node *receiver = context()) const { return runInput(1, receiver); }

    Argument *thirdInput() const { return input(2); }
    bool hasAThirdInput() const { return hasInput(2); }
    Node *runThirdInput(Node *receiver = context()) const { return runInput(2, receiver); }

    ArgumentBunch *outputs(bool createIfNull = true) const {
        if(!_outputs && createIfNull) constCast(this)->_outputs = CHILD_ARGUMENT_BUNCH();
        return _outputs;
    }

    void setOutputs(ArgumentBunch *outputs) { _outputs = outputs; }

    bool isEscaped() const { return _isEscaped; }
    void setIsEscaped(bool isEscaped) { _isEscaped = isEscaped; }

    bool isParented() const { return _isParented; }
    void setIsParented(bool isParented) { _isParented = isParented; }

    bool isVariadic() const { return _isVariadic; }
    void setIsVariadic(bool isVariadic) { _isVariadic = isVariadic; }

    const QString &codeInputName() const { return _codeInputName; }
    void setCodeInputName(const QString &name) { _codeInputName = name; }
    bool hasCodeInput() const { return !_codeInputName.isEmpty(); }

    virtual Node *run(Node *receiver = context()) {
        Node *rcvr = isParented() ? receiver->parent() : receiver;
        Node *result = rcvr->child(name());
        if(!isEscaped()) result = result->run(rcvr, this);
        return result;
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        QString str;
        if(isEscaped())
            str += "\\";
        if(isParented())
            str += "@";
        str += name();
        if(isVariadic())
            str += "...";
        if(inputs(false))
            str += "(" + inputs()->toString(debug, level) + ")";
        if(hasCodeInput())
            str += " " + codeInputName() + "...";
        if(outputs(false))
            str += " -> " + outputs()->toString(debug, level);
        return str;
    }
private:
    QString _name;
    ArgumentBunch *_inputs;
    ArgumentBunch *_outputs;
    bool _isEscaped;
    bool _isParented;
    bool _isVariadic;
    QString _codeInputName;
public:

    // === Message::Sending ===

    #define CHILD_MESSAGE_SENDING(ARGS...) new Message::Sending(Node::context()->child("Message", "Sending"), ##ARGS)

    class Sending : public Node {
        CHILD_DECLARE(Sending, Node);
    public:
        explicit Sending(Node *origin, Message *message = NULL, Node *receiver = NULL) :
            Node(origin), _message(message), _receiver(receiver) {}

        CHILD_FORK_METHOD(Sending, CHILD_FORK_IF_NOT_NULL(_message), CHILD_FORK_IF_NOT_NULL(_receiver));

        virtual Node *receive(Primitive *primitive) {
            return _receiver->child(_message->name())->run(_receiver, _message, primitive);
        }
    private:
        Message *_message;
        Node *_receiver;
    };
};

CHILD_END

#endif // CHILD_MESSAGE_H
