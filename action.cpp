#include <string>
#include <sstream>
#include <iostream>
#include "action.h"
#include "cell.h"

using namespace std;

stack<Action*> undo_stack;
stack<Action*> redo_stack;

//to empty both undo and redo stack
void reset_stacks() {
    //keep popping and deleting action in the redo stack
    while(!redo_stack.empty()) {
        delete redo_stack.top();
        redo_stack.pop();
    }
    
    //keep popping and deleting action in the undo stack
    while(!undo_stack.empty()) {
        delete undo_stack.top();
        undo_stack.pop();
    }
}

//to add action to undo stack (this function to be called when user performed an action)
//input:
//Action* a: the action to be added
void add_action(Action* a) {
    //empty the redo stack
    while(!redo_stack.empty()) {
        delete redo_stack.top();
        redo_stack.pop();
    }
    
    //push the action to the undo stack
    undo_stack.push(a);
}

//pop an action from the undo stack and undo
void undo(vector<vector<Cell>>& board) {
    if(undo_stack.empty()) {
        return;
    }
    
    Action* a = undo_stack.top();
    undo_stack.pop();
    redo_stack.push(a);  //the poped action will be pushed to the redo stack
    board[a->y][a->x].status = a->from; //undo the changes on the board according to the recorded action
}

//pop an action from the redo stack and redo
void redo(vector<vector<Cell>>& board) {
    if(redo_stack.empty()) {
        return;
    }
    
    Action* a = redo_stack.top();
    redo_stack.pop();
    undo_stack.push(a);  //the poped action will be pushed to the undo stack
    board[a->y][a->x].status = a->to;   //redo the action on the board according to the recorded action
}

//get a pointer to the undo_stack as output
stack<Action*>* get_undo_stack() {
    return &undo_stack;
}

//get a pointer to the redo_stack as output
stack<Action*>* get_redo_stack() {
    return &redo_stack;
}

//to convert the action into plain text
//input:
//Action* a: the action to be converted
//output: the created string
string action_to_string(Action* a) {
    stringstream buffer;
    buffer << a->y << " " << a->x << " " << a->from << " " << a->to << endl;
    //e.g.: "2 3 0 1"
    return buffer.str();
}

//to create an action from plain text
//input:
//string s: the string to be converted
//output: the created action
Action* string_to_action(string s) {
    stringstream buffer(s);
    Action* a = new Action();
    buffer >> a->y >> a->x;
    int tmp;
    buffer >> tmp;
    a->from = static_cast<cell_status>(tmp);
    buffer >> tmp;
    a->to = static_cast<cell_status>(tmp);
    return a;
}

//print out the undo and redo stack for debug use
void print_stacks() {
    //duplicate stacks
    stack<Action*> tmp1(undo_stack);
    stack<Action*> tmp2(redo_stack);
    cout << "Undo Stack from top to bottom:" << endl;
    
    while(tmp1.size() > 0) {
        Action* a = tmp1.top();
        cout << "Y: " << a->y << " X: " << a->x << " From: " << a->from << " To: " << a->to << "       " << endl;
        //e.g.: "Y: 2 X: 3 From: 0 To: 1       "
        tmp1.pop();
    }
    
    cout << endl << endl;
    cout << "Redo Stack from top to bottom:" << endl;
    
    while(tmp2.size() > 0) {
        Action* a = tmp2.top();
        cout << "Y: " << a->y << " X: " << a->x << " From: " << a->from << " To: " << a->to << "       " << endl;
        //e.g.: "Y: 2 X: 3 From: 0 To: 1       "
        tmp2.pop();
    }
    
    cout << "======END=====" << endl;
}
