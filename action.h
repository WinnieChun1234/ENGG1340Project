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

void reset_stacks();
void add_action(Action* a);
void undo(vector<vector<Cell>>& board);
void redo(vector<vector<Cell>>& board);
stack<Action*>* get_undo_stack();
stack<Action*>* get_redo_stack();
string action_to_string(Action* a);
Action* string_to_action(string s);
void print_stacks();

#endif
