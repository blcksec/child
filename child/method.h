#ifndef CHILD_METHOD_H
#define CHILD_METHOD_H

#include "child/element.h"
#include "child/block.h"
#include "child/message.h"
#include "child/language/parameter.h"

CHILD_BEGIN

#define CHILD_METHOD(ARGS...) new Method(Node::context()->child("Object", "Method"), ##ARGS)

class Method : public GenericElement<Block *> {
    CHILD_DECLARE(Method, Element);
public:
    explicit Method(const Node *origin, const Block *block = NULL) :
        GenericElement<Block *>(origin, block), _inputs(NULL), _outputs(NULL) {}

    static void initRoot() {
        Object::root()->addChild("Method", root());
        CHILD_NATIVE_METHOD_ADD(Method, init);
    }

    virtual Node *fork() const { return new Method(this, forkIfNotNull(block())); }

    CHILD_NATIVE_METHOD_DECLARE(init) {
        CHILD_CHECK_INPUT_SIZE(0);
        if(message->hasBlock()) setBlock(message->block());
        return this;
    }

    Block *block() const { return value(); } // aliases
    void setBlock(const Block *block) { setValue(block); }

    ParameterList *inputs(bool createIfNull = true) const {
        if(!_inputs && createIfNull) const_cast<Method *>(this)->_inputs = CHILD_PARAMETER_LIST();
        return(_inputs);
    }

    void setInputs(const ParameterList *inputs) { _inputs = inputs; }

    Parameter *input(short i) const { return inputs(false)->get(i); }
    bool hasInput(short i) const { return inputs(false) && inputs()->hasIndex(i); }
    bool hasInput(const QString &label) const { return inputs(false) && inputs()->hasLabel(label); }

    ParameterList *outputs(bool createIfNull = true) const {
        if(!_outputs && createIfNull) const_cast<Method *>(this)->_outputs = CHILD_PARAMETER_LIST();
        return(_outputs);
    }

    void setOutputs(const ParameterList *outputs) { _outputs = outputs; }

    virtual Node *run(const Node *receiver, const Message *message) {
        if(!block()) // method creation
            return Node::run(receiver, message);
        else { // method execution
            Block *forkedBlock = block()->fork();
            QHash<QString, Parameter *> labels(inputs()->labels());
            if(message->inputs(false)) {
                ArgumentBunch::Iterator iterator(message->inputs());
                short i = -1;
                bool labelFound = false;
                while(Argument *argument = iterator.next()) {
                    QString label;
                    if(argument->label()) {
                        label = argument->labelName();
                        if(!hasInput(label)) CHILD_THROW(NotFoundException, "unknown parameter label");
                        if(!labels.contains(label)) CHILD_THROW(DuplicateException, "duplicated parameter label");
                        labelFound = true;
                    } else {
                        if(labelFound) CHILD_THROW(ArgumentException, "positional arguments are forbidden after labeled ones");
                        i++;
                        if(!hasInput(i)) CHILD_THROW(IndexOutOfBoundsException, "too many arguments");
                        label = input(i)->label();
                    }
                    forkedBlock->addChild(label, argument->run());
                    labels.remove(label);
                }
            }
            foreach(Parameter *parameter, labels) {
                if(!parameter->defaultValue()) CHILD_THROW(ArgumentException, "missing mandatory parameter");
                forkedBlock->addChild(parameter->label(), parameter->run());
            }
            ContextPusher pusher(forkedBlock);
            Node *result = NULL;
            try {
                result = forkedBlock->run();
            } catch(Return ret) { result = ret.result; }
            return result;
        }
    }

    virtual void hasBeenAssigned(const Message *message) const {
        if(message->inputs(false)) {
            ArgumentBunch::Iterator i(message->inputs());
            while(Argument *argument = i.next()) {
                PrimitiveChain *label = argument->label();
                PrimitiveChain *defaultValue = argument->value();
                if(!label) {
                    label = defaultValue;
                    defaultValue.clear();
                }
                if(label->size() != 1)
                    CHILD_THROW(ArgumentException, "illegal label parameter found in method definition (should be a Message");
                Message *labelMessage = Message::dynamicCast(label->first()->value());
                if(!labelMessage)
                    CHILD_THROW(ArgumentException, "illegal label parameter found in method definition (should be a Message)");
                inputs()->append(CHILD_PARAMETER(labelMessage->name(), defaultValue));
            }
        }
    }

    virtual QString toString(bool debug = false, short level = 0) const {
        QString str = "Method(";
        if(inputs(false)) str += inputs()->toString(debug, level);
        str += ")";
        if(outputs(false) && outputs()->isNotEmpty())
            str += " -> " + outputs()->toString(debug, level);
        if(block()) str += " " + block()->toString(debug, level + 1);
        return str;
    }
private:
    ParameterList *_inputs;
    ParameterList *_outputs;
};

CHILD_END

#endif // CHILD_METHOD_H
