#ifndef ACTION_H
#define ACTION_H

#include <stack>
#include "cell.h"

using namespace std;

typedef struct {
    int y, x;
    cell_status from;
    cell_status to;
} Action;
void resetStacks();
void addAction(Action* a);
void undo(vector<vector<Cell>>& board);
void redo(vector<vector<Cell>>& board);
stack<Action*>* getUndoStack();
stack<Action*>* getRedoStack();
string actionToStr(Action* a);
Action* stringToAction(string s);
void printStack();

#endif
