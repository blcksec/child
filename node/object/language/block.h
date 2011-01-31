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
        _else(NULL), _elseIsCached(false), _between(NULL), _betweenIsCached(false), _metaSectionsHaveBeenRun(false)  {}

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(Block);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(Block);

    virtual Node *run(Node *receiver = context());

    void runMetaSections(Node *receiver);

    Section *section(const QString &label);
    Section *docSection();
    Section *bodySection();
    Section *testSection();
    Section *elseSection();
    Section *betweenSection();
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
    Section *_between;
    bool _betweenIsCached;
    bool _metaSectionsHaveBeenRun;
};

CHILD_END

#endif // CHILD_BLOCK_H
