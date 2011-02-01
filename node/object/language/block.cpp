#include "node/object/language/block.h"
#include "node/object/boolean.h"
#include "node/object/message.h"
#include "node/object/language/testsuite.h"

CHILD_BEGIN

CHILD_DEFINE(Block, List, Object);

void Block::initRoot() {
    CHILD_ADD_NATIVE_METHOD(Block, doc_section);
    CHILD_ADD_NATIVE_METHOD(Block, body_section);
    CHILD_ADD_NATIVE_METHOD(Block, test_section);
    CHILD_ADD_NATIVE_METHOD(Block, else_section);
    CHILD_ADD_NATIVE_METHOD(Block, before_section);
    CHILD_ADD_NATIVE_METHOD(Block, after_section);
    CHILD_ADD_NATIVE_METHOD(Block, between_section);
}

Node *Block::run(Node *receiver) {
    runMetaSections(receiver);
    if(bodySection()) {
        CHILD_PUSH_RUN(this);
        return bodySection()->run(receiver);
    } else
        return receiver;
}

void Block::runMetaSections(Node *receiver) {
    if(!_metaSectionsHaveBeenRun) {
        if(docSection()) receiver->addOrSetChild("doc", docSection()->run(receiver));
        if(testSection()) {
            TestSuite *testSuite = TestSuite::cast(child("test_suite"));
            testSuite->append(CHILD_TEST(testSection(), receiver));
        }
        _metaSectionsHaveBeenRun = true;
    }
}

Section *Block::section(const QString &label) {
    if(label.isEmpty()) return hasUnlabeledSection();
    else if(label == "doc") return docSection();
    else if(label == "body") return bodySection();
    else if(label == "test") return testSection();
    else if(label == "else") return elseSection();
    else if(label == "before") return beforeSection();
    else if(label == "after") return afterSection();
    else if(label == "between") return betweenSection();
    else return findSection(label);
}

Node *Block::getSection(const QString &label) {
    CHILD_FIND_LAST_MESSAGE;
    CHILD_CHECK_INPUT_SIZE(0);
    Section *sect = section(label);
    if(!sect && !message->isQuestioned()) CHILD_THROW(NotFoundException, QString("'%1' section not found").arg(label));
    if(sect)
        return sect;
    else
        return CHILD_BOOLEAN(false); // FIXME: abstract Section?
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

Section *Block::beforeSection() {
    if(!_beforeIsCached) {
        _before = findSection("before");
        _beforeIsCached = true;
    }
    return _before;
}

Section *Block::afterSection() {
    if(!_afterIsCached) {
        _after = findSection("after");
        _afterIsCached = true;
    }
    return _after;
}

Section *Block::betweenSection() {
    if(!_betweenIsCached) {
        _between = findSection("between");
        _betweenIsCached = true;
    }
    return _between;
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
