#ifndef CHILD_DICTIONARY_TEST_H
#define CHILD_DICTIONARY_TEST_H

#include "node/text.h"
#include "node/dictionary.h"
#include "node/list.h"

#include "tests/testbegin.h"

class DictionaryTest : public QObject {
    Q_OBJECT
private slots:
    void initialize();
    void getAndSet();
    void keys();
    void remove();
    void clear();
};

#include "tests/testend.h"

#endif // CHILD_DICTIONARY_TEST_H
