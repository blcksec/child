#ifndef CHILD_MESSAGE_H
#define CHILD_MESSAGE_H

#include "child/object.h"
#include "child/dictionary.h"
#include "child/block.h"

#define CHILD_MESSAGE(EXPRESSION) static_cast<Message *>(EXPRESSION)

namespace Child {
    class Message : public Object {
    public:
        static Message *root();
        static Message *fork(Node *world) { return(CHILD_MESSAGE(world->child("Message"))->fork()); }

        Message() : _inputs(NULL), _outputs(NULL), _block(NULL) {}

        virtual ~Message() {
            delete _inputs;
            delete _outputs;
            delete _block;
        }

        virtual Message *fork() { return(_fork(this)); }

        const QString &name() const { return(_name); }
        void setName(const QString &name) { _name = name; }

        Dictionary *inputs() const { return(_inputs); }

        void appendInput(const QString &key, PrimitiveChain *input) {
            if(!_inputs) _inputs = Dictionary::fork(this);
            _inputs->set(key, input);
        }

        void appendInput(List *inputs) {
            if(!inputs) throw(NullPointerException("List pointer is NULL"));
            for(int i = 0; i < inputs->size(); i++) {
                appendInput("", CHILD_PRIMITIVECHAIN(inputs->get(i)));
            }
        }

        virtual const QString inspect() const {
            QString str;
            str.append(name());
            if(inputs()) {
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
        static Message *_root;
        QString _name;
        Dictionary *_inputs;
        bool _hasVariadicInputs;
        Dictionary *_outputs;
        bool _hasVariadicOutputs;
        Block *_block;
    };
}

#endif // CHILD_MESSAGE_H
