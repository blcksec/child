#ifndef CHILD_SECTION_H
#define CHILD_SECTION_H

#include "child/object.h"
#include "child/list.h"
#include "child/primitivechain.h"

#define CHILD_SECTION(EXPRESSION) static_cast<Section *>(EXPRESSION)

namespace Child {
    class Section : public Object {
    public:
        static Section *root();
        static Section *fork(Node *world) { return(CHILD_SECTION(world->child("Section"))->fork()); }

        Section() : _primitiveChains(NULL) {}

        virtual ~Section() {
            delete _primitiveChains;
        }

        virtual Section *fork() { return(_fork(this)); }

        const QString &name() const { return(_name); }
        void setName(const QString &name) { _name = name; }

        List *primitiveChains() const { return(_primitiveChains); }

        void appendPrimitiveChain(PrimitiveChain *chain) {
            if(!chain) throw(NullPointerException("PrimitiveChain pointer is NULL"));
            if(!_primitiveChains) _primitiveChains = List::fork(this);
            _primitiveChains->append(chain);
        }

        virtual const QString inspect() const {
            QString str;
            if(!name().isEmpty()) str.append(name() + "\n");
            if(primitiveChains()) {
                for(int i = 0; i < primitiveChains()->size(); i++) {
                    if(i > 0) str.append("\n");
                    QString chain = primitiveChains()->get(i)->inspect();
                    chain.remove(0, 1); // remove useless parenthesis
                    chain.chop(1);
                    str.append("    " + chain);
                }
            }
            return(str);
        }
    private:
        static Section *_root;
        QString _name;
        List *_primitiveChains;
    };
}

#endif // CHILD_SECTION_H
