#ifndef CHILD_RUN_STACK_H
#define CHILD_RUN_STACK_H

#include "node.h"

CHILD_BEGIN

#define CHILD_RUN_STACK(ARGS...) new RunStack(context()->child("RunStack"), ##ARGS)

class RunStack : public Node {
    CHILD_DECLARE(RunStack, Node, Node);
public:
    explicit RunStack(Node *origin) : Node(origin) {}

    CHILD_DECLARE_AND_DEFINE_COPY_METHOD(RunStack);
    CHILD_DECLARE_AND_DEFINE_FORK_METHOD(RunStack);

    void push(Node *node) {
        _stack.push(node);
    }

    Node *pop() {
        if(_stack.isEmpty()) qFatal("Fatal error: run stack is empty!");
        return _stack.pop();
    }

    Node *top() const {
        if(_stack.isEmpty()) qFatal("Fatal error: run stack is empty!");
        return _stack.top();
    }

    template<class T>
    T *find(bool *okPtr = NULL) const {
        T *run = NULL;
        for (int i = _stack.size() - 1; i > 0; --i) {
            run = T::dynamicCast(_stack.at(i));
            if(run) break;
        }
        if(okPtr)
            *okPtr = run;
        else if(!run)
            CHILD_THROW_NOT_FOUND_EXCEPTION("no <nodeName> found in run stack"); // TODO: replace <nodeName> with the class name...
        return run;
    }

    void dump() const;
private:
    QStack<Node *> _stack;
};

inline RunStack *runStack() {
    static RunStack *_stack = new RunStack(RunStack::root());
    return _stack;
}

#define CHILD_PUSH_RUN(NODE) \
RunPusher runPusher(NODE); Q_UNUSED(runPusher);

class RunPusher {
public:
    explicit RunPusher(Node *node) { runStack()->push(node); }
    ~RunPusher() { runStack()->pop(); }
private:
    RunPusher(const RunPusher &); // prevent copying
    RunPusher &operator=(const RunPusher &);
};

CHILD_END

#endif // CHILD_RUN_STACK_H
