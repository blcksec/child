#ifndef CHILD_BLOCK_H
#define CHILD_BLOCK_H

#include "node/object/language/section.h"

CHILD_BEGIN

#define CHILD_BLOCK(ARGS...) \
new Block(context()->child("Object", "Block"), ##ARGS)

class Block : public GenericList<Section *> {
    CHILD_DECLARE(Block, List, Object);
public:
    explicit Block(Node *origin) : GenericList<Section *>(origin),
        _doc(NULL), _docIsCached(false), _body(NULL), _bodyIsCached(false), _test(NULL), _testIsCached(false),
        _else(NULL), _elseIsCached(false),  _before(NULL), _beforeIsCached(false), _after(NULL), _afterIsCached(false),
        _between(NULL), _betweenIsCached(false), _metaSectionsHaveBeenRun(false)  {}

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Block);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Block);

    virtual Node *run(Node *receiver = context());

    void runMetaSections(Node *receiver);

    Section *section(const QString &label);
    Node *getSection(const QString &label);
    Section *docSection();
    CHILD_DECLARE_NATIVE_METHOD(doc_section) { return getSection("doc"); }
    Section *bodySection();
    CHILD_DECLARE_NATIVE_METHOD(body_section) { return getSection("body"); }
    Section *testSection();
    CHILD_DECLARE_NATIVE_METHOD(test_section) { return getSection("test"); }
    Section *elseSection();
    CHILD_DECLARE_NATIVE_METHOD(else_section) { return getSection("else"); }
    Section *beforeSection();
    CHILD_DECLARE_NATIVE_METHOD(before_section) { return getSection("before"); }
    Section *afterSection();
    CHILD_DECLARE_NATIVE_METHOD(after_section) { return getSection("after"); }
    Section *betweenSection();
    CHILD_DECLARE_NATIVE_METHOD(between_section) { return getSection("between"); }
private:
    Section *findSection(const QString &label);
public:
    Section *hasUnlabeledSection();

    virtual QString toString(bool debug = false, short level = 0) const;
private:
    Section *_doc;
    bool _docIsCached;
    Section *_body;
    bool _bodyIsCached;
    Section *_test;
    bool _testIsCached;
    Section *_else;
    bool _elseIsCached;
    Section *_before;
    bool _beforeIsCached;
    Section *_after;
    bool _afterIsCached;
    Section *_between;
    bool _betweenIsCached;
    bool _metaSectionsHaveBeenRun;
};

CHILD_END

#endif // CHILD_BLOCK_H
