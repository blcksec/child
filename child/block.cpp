#include "child/block.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_DEFINE(Block, List);

Section *Block::findSection(const QString &label) {
    Iterator i(this);
    while(Section *section = i.next()) {
        if(section->label() && section->label()->size() == 1) {
            Message *message = Message::dynamicCast(section->label()->first()->value());
            if(message && message->name() == label) return section;
        }
    }
    return NULL;
}

CHILD_END
