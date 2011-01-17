#ifndef CHILD_MESSAGE_H
#define CHILD_MESSAGE_H

#include "node/alias.h"
#include "node/object/language/argumentbunch.h"

CHILD_BEGIN

#define CHILD_MESSAGE(ARGS...) \
(new Message(context()->child("Object", "Message"), ##ARGS))

class Message : public Object {
    CHILD_DECLARE(Message, Object, Object);
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

    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Message, name(), CHILD_FORK_IF_NOT_NULL(inputs(false)), CHILD_FORK_IF_NOT_NULL(outputs(false)),
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

    Argument *lastInput() const { return inputs(false)->last(); }
    Node *runLastInput(Node *receiver = context()) const { return lastInput()->run(receiver); }

    int numInputs() const { return inputs(false) ? inputs()->size() : 0; }

    ArgumentBunch *outputs(bool createIfNull = true) const {
        if(!_outputs && createIfNull) constCast(this)->_outputs = CHILD_ARGUMENT_BUNCH();
        return _outputs;
    }

    void setOutputs(ArgumentBunch *outputs) { _outputs = outputs; }

    Argument *output(short i) const { return outputs(false)->get(i); }
    bool hasOutput(short i) const { return outputs(false) && outputs()->hasIndex(i); }

    Argument *firstOutput() const { return output(0); }
    bool hasAnOutput() const { return hasOutput(0); }

    Argument *secondOutput() const { return output(1); }
    bool hasASecondOutput() const { return hasOutput(1); }

    Argument *thirdOutput() const { return output(2); }
    bool hasAThirdOutput() const { return hasOutput(2); }

    int numOutputs() const { return outputs(false) ? outputs()->size() : 0; }

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
        Alias *alias = Alias::dynamicCast(result);
        if(alias && !alias->target().isEmpty()) result = rcvr->child(alias->target());
        if(!isEscaped()) {
            if(result->isAutoRunnable()) {
                CHILD_PUSH_RUN(this);
                result = result->run(rcvr);
            } else if(inputs(false)) {
                Message *forkMessage = fork();
                forkMessage->setName("fork");
                result = forkMessage->run(result);
            }
        }
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
};

#define CHILD_FIND_LAST_MESSAGE Message *message = findLastMessage();

inline Message *findLastMessage() { return findRun<Message>(); }

CHILD_END

#endif // CHILD_MESSAGE_H
