#ifndef __UNDO_STACK_HPP__
#define __UNDO_STACK_HPP__

#include <list>
#include <stack>

class JointNode;

struct UndoAction {
    JointNode* joint;
    char axis;
    double from;
    double to;

    UndoAction();
    UndoAction(JointNode* joint, char axis, double from, double to);
    UndoAction(const UndoAction& other);
};

typedef std::list<UndoAction> UndoStep;

void updateStepRotation(UndoStep& step, JointNode* joint, char axis, double from, double to);

class UndoStack {
    std::stack<UndoStep> undoStack;
    std::stack<UndoStep> redoStack;

public:
    UndoStack();

    bool undo();
    bool redo();

    int getUndoStackSize() const;
    int getRedoStackSize() const;

    void clear();

    void push(const UndoStep& step);
};

#endif
