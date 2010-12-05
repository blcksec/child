#ifndef CHILD_H
#define CHILD_H

//#define CHILD_IS_NAMESPACED

#ifdef CHILD_IS_NAMESPACED
#define CHILD_BEGIN namespace Child {
#define CHILD_END }
#else
#define CHILD_BEGIN
#define CHILD_END
#endif

#ifdef CHILD_IS_NAMESPACED
#define CHILD_USE using namespace Child;
#else
#define CHILD_USE
#endif


CHILD_BEGIN

void init();

CHILD_END

#endif // CHILD_H
