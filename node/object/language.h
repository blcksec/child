#ifndef CHILD_LANGUAGE_H
#define CHILD_LANGUAGE_H

#include "node/object.h"

CHILD_BEGIN

namespace Language {
    Object *root();
//    static bool isInitialized = root();
}

using namespace Language;

CHILD_END

#endif // CHILD_LANGUAGE_H
