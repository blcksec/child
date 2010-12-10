#ifndef CHILD_LANGUAGE_H
#define CHILD_LANGUAGE_H

#include "child/object.h"

CHILD_BEGIN

namespace Language {
    ObjectPtr &root();
    static bool isInitialized = root().isNotNull();
}

CHILD_END

#endif // CHILD_LANGUAGE_H
