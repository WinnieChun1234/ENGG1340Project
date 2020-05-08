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
    //keep pop and delete action in redo stack
    while(!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }
    
    //keep pop and delete action in undo stack
    while(!undoStack.empty()) {
        delete undoStack.top();
        undoStack.pop();
    }
}

//to add action to undo stack (this function to be called when user performed an action)
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

stack<Action*>* getUndoStack() {
    return &undoStack;
}

stack<Action*>* getRedoStack() {
    return &redoStack;
}

//to convert the action into plain text
string actionToStr(Action* a) {
//  return string(a->y+"|"+a->x+"|"+a->from+"|"+a->to);
    stringstream buffer;
    buffer << a->y << " " << a->x << " " << a->from << " " << a->to << endl;
    return buffer.str();
}

//to create an action from plain text
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
        tmp1.pop();
    }
    
    cout << endl << endl;
    cout << "Redo Stack from top to bottom:" << endl;
    
    while(tmp2.size() > 0) {
        Action* a = tmp2.top();
        cout << "Y: " << a->y << " X: " << a->x << " From: " << a->from << " To: " << a->to << "       " << endl;
        tmp2.pop();
    }
    
    cout << "======END=====" << endl;
}
