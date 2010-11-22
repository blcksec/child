#ifndef CHILD_MESSAGE_H
#define CHILD_MESSAGE_H

#include "child/object.h"
#include "child/dictionary.h"
#include "child/block.h"

namespace Child {
    class Message : public Object {
        CHILD_DECLARATION(Message);
    public:
        static Message *is(Node *node, const QString &name) {
            Message *message = Message::is(node);
            return(message && message->name() == name ? message : NULL);
        }

        Message() : _inputs(NULL), _outputs(NULL), _block(NULL) {}

        virtual ~Message() {
            delete _inputs;
            delete _outputs;
            delete _block;
        }

        const QString &name() const { return(_name); }
        void setName(const QString &name) { _name = name; }

        Dictionary *inputs(bool createIfNull = false) const {
            if(_inputs) return(_inputs);
            if(createIfNull)
                return(const_cast<Message *>(this)->_inputs =
                       Dictionary::fork(const_cast<Message *>(this)));
            else
                return(Dictionary::root());
        }

        void appendInput(const QString &key, PrimitiveChain *input) {
            inputs(true)->set(key, input);
        }

        void appendInput(List *inputs) {
            if(!inputs) throw(NullPointerException("List pointer is NULL"));
            for(int i = 0; i < inputs->size(); i++) {
                appendInput("", PrimitiveChain::as(inputs->get(i)));
            }
        }

        PrimitiveChain *input(int i) const { return(PrimitiveChain::as(inputs()->get(i))); }
        PrimitiveChain *firstInput() const { return(input(0)); }
        PrimitiveChain *secondInput() const { return(input(1)); }
        PrimitiveChain *lastInput() const { return(input(inputCount()-1)); }

        int inputCount() const { return(inputs()->size()); }

        virtual const QString inspect() const {
            QString str;
            str.append(name());
            if(inputs()->isNotEmpty()) {
                str.append("(");
                bool first = true;
                foreach(NamedNode namedNode, inputs()->namedNodes()) {
                    if(!first) str.append(", "); else first = false;
                    if(!Dictionary::keyIsAnonymous(namedNode.name)) {
                        str.append(Dictionary::unescapedKey(namedNode.name));
                        str.append(": ");
                    }
                    QString chain = namedNode.node->inspect();
                    chain.remove(0, 1); // remove useless parenthesis
                    chain.chop(1);
                    str.append(chain);
                }
                str.append(")");
            }
            return(str);
        }
    private:
        QString _name;
        Dictionary *_inputs;
        bool _hasVariadicInputs;
        Dictionary *_outputs;
        bool _hasVariadicOutputs;
        Block *_block;
    };
}

#endif // CHILD_MESSAGE_H
