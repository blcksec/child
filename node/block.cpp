#include "child/block.h"
#include "child/message.h"
#include "child/language/testsuite.h"

CHILD_BEGIN

CHILD_DEFINE(Block, List);

void Block::initRoot() {
    Object::root()->addChild("Block", this);
}

Node *Block::run(Node *receiver) {
    if(docSection()) receiver->addOrSetChild("doc", docSection()->run(receiver));
    if(testSection()) {
        TestSuite *testSuite = TestSuite::cast(child("test_suite"));
        testSuite->append(CHILD_TEST(testSection(), receiver));
    }
    if(bodySection()) bodySection()->run(receiver);
    return receiver;
}

Section *Block::section(const QString &label) {
    if(label.isEmpty()) return hasUnlabeledSection();
    else if(label == "doc") return docSection();
    else if(label == "body") return bodySection();
    else if(label == "test") return testSection();
    else if(label == "else") return elseSection();
    else return findSection(label);
}

Section *Block::docSection() {
    if(!_docIsCached) {
        _doc = findSection("doc");
        if(!_doc && findSection("body")) _doc = hasUnlabeledSection();
        _docIsCached = true;
    }
    return _doc;
}

Section *Block::bodySection() {
    if(!_bodyIsCached) {
        _body = findSection("body");
        if(!_body) _body = hasUnlabeledSection();
        _bodyIsCached = true;
    }
    return _body;
}

Section *Block::testSection() {
    if(!_testIsCached) {
        _test = findSection("test");
        _testIsCached = true;
    }
    return _test;
}

Section *Block::elseSection() {
    if(!_elseIsCached) {
        _else = findSection("else");
        _elseIsCached = true;
    }
    return _else;
}

Section *Block::findSection(const QString &label) {
    Iterator i(this);
    while(Section *section = i.next()) {
        if(section->label() && !section->label()->hasNext()) {
            Message *message = Message::dynamicCast(section->label()->value());
            if(message && message->name() == label) return section;
        }
    }
    return NULL;
}

Section *Block::hasUnlabeledSection() {
    if(isNotEmpty() && !first()->label())
        return first();
    else
        return NULL;
}

QString Block::toString(bool debug, short level) const {
    QString str;
    if(level > 0) str += "{\n";
    str += join("\n", "", "", debug, level + 1);
    if(level > 0) str += "\n" + QString("    ").repeated(level) + "}";
    return str;
}

CHILD_END
