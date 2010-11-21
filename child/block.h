#ifndef CHILD_BLOCK_H
#define CHILD_BLOCK_H

#include "child/object.h"
#include "child/dictionary.h"
#include "child/section.h"

#define CHILD_BLOCK(EXPRESSION) static_cast<Block *>(EXPRESSION)

namespace Child {
    class Block : public Object {
    public:
        static Block *root();
        static Block *fork(Node *world) { return(CHILD_BLOCK(world->child("Block"))->fork()); }

        Block() : _sections(NULL) {}

        virtual ~Block() {
            delete _sections;
        }

        virtual Block *fork() { return(_fork(this)); }

        Dictionary *sections() const { return(_sections); }

        void appendSection(Section *section) {
            if(!section) throw(NullPointerException("Section pointer is NULL"));
            if(!_sections) _sections = Dictionary::fork(this);
            _sections->set(section->name(), section);
        }

        virtual const QString inspect() const {
            if(!sections()) return("");
            QString str;
            bool first = true;
            foreach(QString label, sections()->keys()) {
                if(!first) str.append("\n"); else first = false;
                str.append(const_cast<Dictionary *>(sections())->get(label)->inspect());
            }
            return(str);
        }
    private:
        static Block *_root;
        Dictionary *_sections;
    };
}

#endif // CHILD_BLOCK_H
