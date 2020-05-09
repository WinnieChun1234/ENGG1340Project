#include <string>
#include <sstream>
#include <iostream>
#include "action.h"
#include "cell.h"

using namespace std;

stack<Action*> undoStack;
stack<Action*> redoStack;

//to empty both undo and redo stack
void resetStacks() {
    //keep popping and deleting action in the redo stack
    while(!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }

    //keep popping and deleting action in the undo stack
    while(!undoStack.empty()) {
        delete undoStack.top();
        undoStack.pop();
    }
}

//to add action to undo stack (this function to be called when user performed an action)
//input:
//Action* a: the action to be added
void addAction(Action* a) {
    //empty the redo stack
    while(!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }

    //push the action to the undo stack
    undoStack.push(a);
}

//pop an action from the undo stack and undo
void undo(vector<vector<Cell>>& board) {
    if(undoStack.empty()) {
        return;
    }

    Action* a = undoStack.top();
    undoStack.pop();
    redoStack.push(a);  //the poped action will be pushed to the redo stack
    board[a->y][a->x].status = a->from; //undo the changes on the board according to the recorded action
}

//pop an action from the redo stack and redo
void redo(vector<vector<Cell>>& board) {
    if(redoStack.empty()) {
        return;
    }

    Action* a = redoStack.top();
    redoStack.pop();
    undoStack.push(a);  //the poped action will be pushed to the undo stack
    board[a->y][a->x].status = a->to;   //redo the action on the board according to the recorded action
}

//get a pointer to the undoStack as output
stack<Action*>* getUndoStack() {
    return &undoStack;
}

//get a pointer to the redoStack as output
stack<Action*>* getRedoStack() {
    return &redoStack;
}

//to convert the action into plain text
//input:
//Action* a: the action to be converted
//output: the created string
string actionToStr(Action* a) {
    stringstream buffer;
    buffer << a->y << " " << a->x << " " << a->from << " " << a->to << endl;
    //e.g.: "2 3 0 1"
    return buffer.str();
}

//to create an action from plain text
//input:
//string s: the string to be converted
//output: the created action
Action* stringToAction(string s) {
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
void printStack() {
    //duplicate stacks
    stack<Action*> tmp1(undoStack);
    stack<Action*> tmp2(redoStack);
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
