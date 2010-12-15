#include "child/block.h"
#include "child/message.h"

CHILD_BEGIN

CHILD_DEFINE(Block, List);

SectionPointer Block::findSection(const QString &label) {
    Iterator i(this);
    while(SectionPointer section = i.next()) {
        if(section->label() && section->label()->size() == 1) {
            MessagePointer message(section->label()->first()->value(), true);
            if(message && message->name() == label) return section;
        }
    }
    return SectionPointer::null();
}

CHILD_END
