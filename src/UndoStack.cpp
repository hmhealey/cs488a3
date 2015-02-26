#include "UndoStack.hpp"

#include "Scene.hpp"

using namespace std;

UndoAction::UndoAction() { }

UndoAction::UndoAction(JointNode* joint, char axis, double from, double to) : joint(joint), axis(axis), from(from), to(to) { }

UndoAction::UndoAction(const UndoAction& other) {
    this->joint = other.joint;
    this->axis = other.axis;
    this->from = other.from;
    this->to = other.to;
}

void updateStepRotation(UndoStep& step, JointNode* joint, char axis, double from, double to) {
    for (auto i = step.begin(); i != step.end(); i++) {
        if (i->joint == joint && i->axis == axis) {
            // we've found an existing rotation action for this joint/axis combination so just update it
            // and return
            i->to = to;
            return;
        }
    }

    // we didn't find an existing action so we need to add a new one
    step.emplace_back(joint, axis, from, to);
}

UndoStack::UndoStack() { }

bool UndoStack::undo() {
    if (!undoStack.empty()) {
        // c++ stacks are silly
        UndoStep step = undoStack.top();
        undoStack.pop();

        for (auto i = step.cbegin(); i != step.cend(); i++) {
            switch(i->axis) {
            case 'x':
                i->joint->setXRotation(i->from);
                break;
            case 'y':
                i->joint->setYRotation(i->from);
                break;
            default:
                cerr << "UndoStack::undo - Unable to undo rotation around " << i->axis << " axis" << endl;
                break;
            }
        }

        redoStack.push(step);

        return true;
    } else {
        return false;
    }
}

bool UndoStack::redo() {
    if (!redoStack.empty()) {
        // c++ stacks are silly
        UndoStep step = redoStack.top();
        redoStack.pop();

        for (auto i = step.cbegin(); i != step.cend(); i++) {
            switch(i->axis) {
            case 'x':
                i->joint->setXRotation(i->to);
                break;
            case 'y':
                i->joint->setYRotation(i->to);
                break;
            default:
                cerr << "UndoStack::redo - Unable to undo rotation around " << i->axis << " axis" << endl;
                break;
            }
        }

        undoStack.push(step);

        return true;
    } else {
        return false;
    }
}

int UndoStack::getUndoStackSize() const {
    return undoStack.size();
}

int UndoStack::getRedoStackSize() const {
    return redoStack.size();
}

void UndoStack::clear() {
    while (!undoStack.empty()) undoStack.pop();
    while (!redoStack.empty()) redoStack.pop();
}

void UndoStack::push(const UndoStep& step) {
    // dump the redo stack since it's now invalid
    while (!redoStack.empty()) redoStack.pop();

    undoStack.push(step);
}
