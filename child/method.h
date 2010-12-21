#ifndef CHILD_METHOD_H
#define CHILD_METHOD_H

#include "child/element.h"
#include "child/block.h"
#include "child/message.h"
#include "child/language/parameter.h"

CHILD_BEGIN

CHILD_POINTER_DECLARE(Method, Element);

#define CHILD_METHOD(ARGS...) MethodPointer(new Method(Node::context()->child("Object", "Method"), ##ARGS))

class Method : public GenericElement<MethodPointer, BlockPointer> {
    CHILD_DECLARE(Method, Element);
public:
    explicit Method(const Pointer &origin, const BlockPointer &block = BlockPointer::null()) :
        GenericElement<MethodPointer, BlockPointer>(origin, block) {}

    static void initRoot() {
        Object::root()->addChild("Method", root());
        CHILD_NATIVE_METHOD_ADD(Method, init);
    }

    virtual Pointer fork() const { return new Method(this, forkIfNotNull(block())); }

    CHILD_NATIVE_METHOD_DECLARE(init) {
        CHILD_CHECK_INPUT_SIZE(0);
        if(message->hasBlock()) setBlock(message->block());
        return this;
    }

    BlockPointer block() const { return value(); } // aliases
    void setBlock(const BlockPointer &block) { setValue(block); }

    ParameterListPointer inputs(bool createIfNull = true) const {
        if(!_inputs && createIfNull) const_cast<Method *>(this)->_inputs = CHILD_PARAMETER_LIST();
        return(_inputs);
    }

    void setInputs(const ParameterListPointer &inputs) { _inputs = inputs; }

    ParameterPointer input(short i) const { return inputs(false)->get(i); }
    bool hasInput(short i) const { return inputs(false) && inputs()->hasIndex(i); }
    bool hasInput(const QString &label) const { return inputs(false) && inputs()->hasLabel(label); }

    ParameterListPointer outputs(bool createIfNull = true) const {
        if(!_outputs && createIfNull) const_cast<Method *>(this)->_outputs = CHILD_PARAMETER_LIST();
        return(_outputs);
    }

    void setOutputs(const ParameterListPointer &outputs) { _outputs = outputs; }

    virtual Pointer run(const Pointer &receiver, const MessagePointer &message) {
        if(!block()) // method creation
            return Node::run(receiver, message);
        else { // method execution
            BlockPointer forkedBlock = block()->fork();
            QHash<QString, ParameterPointer> labels(inputs()->labels());
            if(message->inputs(false)) {
                ArgumentBunch::Iterator iterator(message->inputs());
                short i = -1;
                bool labelFound = false;
                while(ArgumentPointer argument = iterator.next()) {
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
            foreach(ParameterPointer parameter, labels) {
                if(!parameter->defaultValue()) CHILD_THROW(ArgumentException, "missing mandatory parameter");
                forkedBlock->addChild(parameter->label(), parameter->run());
            }
            ContextPusher pusher(forkedBlock);
            Pointer result;
            try {
                result = forkedBlock->run();
            } catch(Return ret) { result = ret.result; }
            return result;
        }
    }

    virtual void hasBeenAssigned(const MessagePointer &message) const {
        if(message->inputs(false)) {
            ArgumentBunch::Iterator i(message->inputs());
            while(ArgumentPointer argument = i.next()) {
                PrimitiveChainPointer label = argument->label();
                PrimitiveChainPointer defaultValue = argument->value();
                if(!label) {
                    label = defaultValue;
                    defaultValue.clear();
                }
                if(label->size() != 1)
                    CHILD_THROW(ArgumentException, "illegal label parameter found in method definition (should be a Message");
                MessagePointer labelMessage(label->first()->value(), true);
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
    ParameterListPointer _inputs;
    ParameterListPointer _outputs;
};

CHILD_POINTER_DEFINE(Method, Element);

CHILD_END

#endif // CHILD_METHOD_H
