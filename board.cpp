#include <time.h>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <fstream>
#include "cell.h"
#include "board.h"

using namespace std;

//flood all the non-black cell into black from the position i,j of the board
//inputs:
//board: the board to be checked
//i, j: the coordinates of the starting cell
void blackTheCell(vector<vector<Cell>>& board, int i, int j) {
    if(board[i][j].status == BLACK) {
        return;
    }

    int w = board.size();
    board[i][j].status = BLACK; //check the cell (i,j) into black

    //for each of the four neighbor cell, if they are not out of the board, flood it
    if(i > 0) {
        blackTheCell(board, i - 1, j);
    }

    if(i < w - 1) {
        blackTheCell(board, i + 1, j);
    }

    if(j > 0) {
        blackTheCell(board, i, j - 1);
    }

    if(j < w - 1) {
        blackTheCell(board, i, j + 1);
    }
}

//check if all white cell are connected and output the result if out=true
bool checkWhite(vector<vector<Cell>> board, bool out) {
    if(!checkWhiteConnected(board)) {
        if(out) {
            printf("All white cell should be connected each others\n");
        }

        return false;
    }

    return true;
}

//check if all white (non-black) cells are connected and return true/false
bool checkWhiteConnected(vector<vector<Cell>> board) {
    int w = board.size();
    int i1 = -1, j1 = -1;

    //loop the board until we find a non-black cell
    for(int i = 0; i < w && i1 == -1; i++) {
        for(int j = 0; j < w && i1 == -1; j++) {
            if(board[i][j].status != BLACK) {
                i1 = i;
                j1 = j;
            }
        }
    }

    if(i1 == -1) { //there are only black cells
        return true;
    }

    //flood all the non-black cell into black starting form the first non-black cell found
    blackTheCell(board, i1, j1);

    //if all white (non-black) cell are connected, there should be no non-black cell after flooding
    //so, here we loop thru the board and return false if there exists non-black cell
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < w; j++) {
            if(board[i][j].status != BLACK) {
                return false;
            }
        }
    }

    //otherwise, return true
    return true;
}

//generate a new board
//inputs:
//w: the board size
//debug_mode: enable/disable logging into file
vector<vector<Cell>> generate_board(int w, bool debug_mode) {
    srand(time(NULL));      //initialize random number generator
    vector<int> num(w);     //vector to store the number to be used

    //fill in the vector with 1..w
    for(int i = 0; i < w; i++) {
        num[i] = i + 1;
    }

    //randonize the number order in the vector
    for(int i = 0; i < (w * 2); i++) {
        swap(num[rand() % w], num[rand() % w]);
    }

    //vector of vector of cell to store the board data
    vector<vector<Cell>> board(w);

    //fill in the board with unmarked cell with numbers
    //the board will be a latin square
    for(int i = 0; i < w; i++) {
        vector<Cell> m(w);

        for(int j = 0; j < w; j++) {
            m[j].n = num[(i + j) % w];
            m[j].status = UNMARKED;
        }

        board[i] = m;
    }

    //randomly swap some rows in the board (swapping rows in a latin square is still a valid latin square)
    for(int i = 0; i < (w * 2); i++) {
        swap(board[rand() % w], board[rand() % w]);
    }

    //randomly swap some column in the board (swapping column in a latin square is still a valid latin square)
    for(int i = 0; i < (w * 2); i++) {
        int a = rand() % w;
        int b = rand() % w;

        for(int j = 0; j < w; j++) {
            swap(board[j][a], board[j][b]);
        }
    }

    //mark a random amount of cells as black without breaking the rule, and
    //change the number of black cell to create duplcates.
    for(int i = 0; i < (w * w / 2); i++) {
        int y, x;

        //randomly pick a unmarked cell
        do {
            y = rand() % w;
            x = rand() % w;
        } while(board[y][x].status != UNMARKED);

        //not to black this cell if any of its neighbor cell are black

        if(y > 0 && board[y - 1][x].status == BLACK) {
            continue;
        }

        if(y < w - 1 && board[y + 1][x].status == BLACK) {
            continue;
        }

        if(x > 0 && board[y][x - 1].status == BLACK) {
            continue;
        }

        if(x < w - 1 && board[y][x + 1].status == BLACK) {
            continue;
        }

        //change this cell into black
        board[y][x].status = BLACK;
        if(checkWhiteConnected(board)) { //if all white cells in the board are connected, change the number of the cell
            board[y][x].n = num[rand() % w];
        } else {  //otherwise, restore the cell
            board[y][x].status = UNMARKED;
        }
    }

    //the question and the answer will be outputted into the log file if debug mode is enabled
    if(debug_mode) {
        ofstream of;
        of.open("log.log", ofstream::app);
        of << "Question :" << endl;

        for(int i = 0; i < board.size(); i++) {
            for(int j = 0; j < board[0].size(); j++) {
                of << board[i][j].n;
            }

            of << endl;
        }

        of << "\n\nAnswer:" << endl;

        for(int i = 0; i < board.size(); i++) {
            for(int j = 0; j < board[0].size(); j++) {
                if(board[i][j].status == BLACK) {
                    of << "█";
                } else {
                    of << board[i][j].n;
                }
            }

            of << endl;
        }

        of << endl << endl << endl;
        of.close();
    }

    //board is now a solution, change it back to question:
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < w; j++) {
            board[i][j].status = UNMARKED;
        }
    }

    return board;
}

//check if there are any duplicated numbers in any rows
//inputs:
//board: the board to be checked
//out: enable/disable logging onto screen
bool checkRows(vector<vector<Cell>> board, bool out) {
    //create a vector of size "board[0].size()" filled with "false"
    vector<bool> num(board[0].size());

    for(int i = 0; i < num.size(); i++) {
        num[i] = false;
    }

    for(int i = 0; i < board.size(); i++) { //for each row
        vector<bool> num2 = num;    //assume that all number are not exists in the board first

        for(int j = 0; j < board[i].size(); j++) { //for each cell in this row
            if(board[i][j].status == BLACK) {   //if the cell is black, ignore its number
                continue;
            }

            if(num2[board[i][j].n]) {   //if the number already existed before, this number is duplicated
                if(out) {
                    printf("Duplicated number %d in row %d\n", board[i][j].n, i + 1);
                }

                return false;
            } else {    //otherwise, mark this number existed
                num2[board[i][j].n] = true;
            }
        }
    }

    return true;
}

//check if there are any duplicated numbers in any column
//inputs:
//board: the board to be checked
//out: enable/disable logging onto screen
bool checkColumns(vector<vector<Cell>> board, bool out) {
    //create a vector of size "board[0].size()" filled with "false"
    vector<bool> num(board.size());

    for(int i = 0; i < num.size(); i++) {
        num[i] = false;
    }

    for(int i = 0; i < board[0].size(); i++) {  //for each column
        vector<bool> num2 = num;  //assume that all number are not exists in the board first

        for(int j = 0; j < board.size(); j++) { //for each cell in this column
            if(board[j][i].status == BLACK) {    //if the cell is black, ignore its number
                continue;
            }

            if(num2[board[j][i].n]) {  //if the number already existed before, this number is duplicated
                if(out) {
                    printf("Duplicated number %d in column %c\n", board[j][i].n, i + 'A');
                }

                return false;
            } else {  //otherwise, mark this number existed
                num2[board[j][i].n] = true;
            }
        }
    }

    return true;
}

//check if there are any adjacent black cell
//inputs:
//board: the board to be checked
//out: enable/disable logging onto screen
bool checkAdjBlackCell(vector<vector<Cell>> board, bool out) {
    //for each row, check if there are any adjacent black cell
    for(int i = 0; i < board.size(); i++) {
        for(int j = 1; j < board[i].size(); j++) {
            if(board[i][j].status == BLACK && board[i][j - 1].status == BLACK) {
                if(out) {
                    printf("Adjacent black cell in row %d\n", i + 1);
                }

                return false;
            }
        }
    }

    //for each column, check if there are any adjacent black cell
    for(int i = 0; i < board[0].size(); i++) {
        for(int j = 1; j < board.size(); j++) {
            if(board[j][i].status == BLACK && board[j - 1][i].status == BLACK) {
                if(out) {
                    printf("Adjacent black cell in column %c\n", i + 'A');
                }

                return false;
            }
        }
    }

    return true;
}
