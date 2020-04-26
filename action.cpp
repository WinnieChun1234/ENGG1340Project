#include <string>
#include <sstream>
#include <iostream>
#include "action.h"
#include "cell.h"

using namespace std;

stack<Action*> undoStack;
stack<Action*> redoStack;

void resetStacks() {
    while(!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }
    
    while(!undoStack.empty()) {
        delete undoStack.top();
        undoStack.pop();
    }
}

void addAction(Action* a) {
    while(!redoStack.empty()) {
        delete redoStack.top();
        redoStack.pop();
    }
    
    undoStack.push(a);
}

void undo(vector<vector<Cell>>& board) {
    if(undoStack.empty()) {
        return;
    }
    
    Action* a = undoStack.top();
    undoStack.pop();
    redoStack.push(a);
    board[a->y][a->x].status = a->from;
}

void redo(vector<vector<Cell>>& board) {
    if(redoStack.empty()) {
        return;
    }
    
    Action* a = redoStack.top();
    redoStack.pop();
    undoStack.push(a);
    board[a->y][a->x].status = a->to;
}

stack<Action*>* getUndoStack() {
    return &undoStack;
}

stack<Action*>* getRedoStack() {
    return &redoStack;
}

string actionToStr(Action* a) {
//  return string(a->y+"|"+a->x+"|"+a->from+"|"+a->to);
    stringstream buffer;
    buffer << a->y << " " << a->x << " " << a->from << " " << a->to << endl;
    return buffer.str();
}

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

void printStack() {
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
