#include <time.h>
#include <cstdlib>
#include <vector>
#include <cstdio>
#include <fstream>
#include "cell.h"
#include "board.h"

using namespace std;

void blackTheCell(vector<vector<Cell>>& board, int i, int j) {
    if(board[i][j].status == BLACK) {
        return;
    }
    
    int w = board.size();
    board[i][j].status = BLACK;
    
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

bool checkWhite(vector<vector<Cell>> board, bool out) {
    if(!checkWhiteConnected(board)) {
        if(out) {
            printf("All white cell should be connected each others\n");
        }
        
        return false;
    }
    
    return true;
}
bool checkWhiteConnected(vector<vector<Cell>> board) {
    int w = board.size();
    int i1 = -1, j1 = -1;
    
    for(int i = 0; i < w && i1 == -1; i++) {
        for(int j = 0; j < w && i1 == -1; j++) {
            if(board[i][j].status != BLACK) {
                i1 = i;
                j1 = j;
            }
        }
    }
    
    blackTheCell(board, i1, j1);
    
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < w; j++) {
            if(board[i][j].status != BLACK) {
                return false;
            }
        }
    }
    
    return true;
}

vector<vector<Cell>> generate_board(int w, bool debug_mode) {
    srand(time(NULL));
    vector<int> num(w);
    
    for(int i = 0; i < w; i++) {
        num[i] = i + 1;
    }
    
    for(int i = 0; i < (w * 2); i++) {
        swap(num[rand() % w], num[rand() % w]);
    }
    
    vector<vector<Cell>> board(w);
    
    for(int i = 0; i < w; i++) {
        vector<Cell> m(w);
        
        for(int j = 0; j < w; j++) {
            m[j].n = num[(i + j) % w];
            m[j].status = UNMARKED;
        }
        
        board[i] = m;
    }
    
    for(int i = 0; i < (w * 2); i++) {
        swap(board[rand() % w], board[rand() % w]);
    }
    
    for(int i = 0; i < (w * 2); i++) {
        int a = rand() % w;
        int b = rand() % w;
        
        for(int j = 0; j < w; j++) {
            swap(board[j][a], board[j][b]);
        }
    }
    
    for(int i = 0; i < (w * w / 2); i++) {
        int y, x;
        
        do {
            y = rand() % w;
            x = rand() % w;
        } while(board[y][x].status != UNMARKED);
        
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
        
        board[y][x].status = BLACK;
        
        if(checkWhiteConnected(board)) {
            board[y][x].n = num[rand() % w];
        }
        else {
            board[y][x].status = UNMARKED;
        }
    }
    
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

bool checkRows(vector<vector<Cell>> board, bool out) {
    vector<bool> num(board[0].size());
    
    for(int i = 0; i < num.size(); i++) {
        num[i] = false;
    }
    
    for(int i = 0; i < board.size(); i++) {
        vector<bool> num2 = num;
        
        for(int j = 0; j < board[i].size(); j++) {
            if(board[i][j].status == BLACK) {
                continue;
            }
            
            if(num2[board[i][j].n]) {
                if(out) {
                    printf("Duplicated number %d in row %d\n", board[i][j].n, i + 1);
                }
                
                return false;
            } else {
                num2[board[i][j].n] = true;
            }
        }
    }
    
    return true;
}

bool checkColumns(vector<vector<Cell>> board, bool out) {
    vector<bool> num(board.size());
    
    for(int i = 0; i < num.size(); i++) {
        num[i] = false;
    }
    
    for(int i = 0; i < board[0].size(); i++) {
        vector<bool> num2 = num;
        
        for(int j = 0; j < board.size(); j++) {
            if(board[j][i].status == BLACK) {
                continue;
            }
            
            if(num2[board[j][i].n]) {
                if(out) {
                    printf("Duplicated number %d in column %c\n", board[j][i].n, i + 'A');
                }
                
                return false;
            } else {
                num2[board[j][i].n] = true;
            }
        }
    }
    
    return true;
}

bool checkAdjBlackCell(vector<vector<Cell>> board, bool out) {
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
