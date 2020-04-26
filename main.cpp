#include <iostream>
#include <fstream>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stack>
#include "board.h"
#include "action.h"
using namespace std;

//////// BELOW FUNCTION MODIFIED FROM https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar
int readkey() {
    int c;
    static struct termios oldt, newt;
    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr(STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;
    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON | ECHO);
    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    c = getchar();
    /*restore the old settings*/
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return c;
}
//////// ABOVE FUNCTION MODIFIED FROM https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar

enum GAME_MODE { PRACTICAL = 1, NORMAL = 2 };
void start_tutorial();
void start_game(GAME_MODE, vector<vector<Cell>>* savedBoard = NULL);
void settings_screen();
void load_screen();
void exit_screen();
void exportGame(vector<vector<Cell>> board, GAME_MODE mode);

void cls() { //Clear Screen
    cout << "\033[2J\033[1;1H";
}

int board_size = 9;
int cur_i, cur_j;
bool debug_mode = false;

int main(int argc, char* argv[]) {
    if(argc > 1 && !strcmp(argv[1], "-d")) {
        debug_mode = true;
    }
    
    string c;
    
    while(1) {
        cls();
        
        if(debug_mode) {
            cout << "debug mode enabled" << endl;
        }
        
        cout << endl;
        cout << "   ██████  ▒█████   ██▓     ██▓▄▄▄█████▓ █    ██ ▓█████▄  ▒█████   " << endl;
        cout << "▒██    ▒ ▒██▒  ██▒▓██▒    ▓██▒▓  ██▒ ▓▒ ██  ▓██▒▒██▀ ██▌▒██▒  ██▒  " << endl;
        cout << "░ ▓██▄   ▒██░  ██▒▒██░    ▒██▒▒ ▓██░ ▒░▓██  ▒██░░██   █▌▒██░  ██▒  " << endl;
        cout << "  ▒   ██▒▒██   ██░▒██░    ░██░░ ▓██▓ ░ ▓▓█  ░██░░▓█▄   ▌▒██   ██░  " << endl;
        cout << "▒██████▒▒░ ████▓▒░░██████▒░██░  ▒██▒ ░ ▒▒█████▓ ░▒████▓ ░ ████▓▒░  " << endl;
        cout << "▒ ▒▓▒ ▒ ░░ ▒░▒░▒░ ░ ▒░▓  ░░▓    ▒ ░░   ░▒▓▒ ▒ ▒  ▒▒▓  ▒ ░ ▒░▒░▒░   " << endl;
        cout << "░ ░▒  ░ ░  ░ ▒ ▒░ ░ ░ ▒  ░ ▒ ░    ░    ░░▒░ ░ ░  ░ ▒  ▒   ░ ▒ ▒░   " << endl;
        cout << "░  ░  ░  ░ ░ ░ ▒    ░ ░    ▒ ░  ░       ░░░ ░ ░  ░ ░  ░ ░ ░ ░ ▒    " << endl;
        cout << "      ░      ░ ░      ░  ░ ░              ░        ░        ░ ░    " << endl;
        cout << "                                                 ░         By Winnie & Donald" << endl;
        cout << "Select Select:" << endl;
        cout << "1. Tutorial Mode" << endl;
        cout << "2. Practical Mode" << endl;
        cout << "3. Normal Mode" << endl;
        cout << "4. Settings" << endl;
        cout << "5. Load Saved Game" << endl;
        cout << "6. Exit" << endl;
        cout << endl;
        
        while(1) {
            cout << "Please enter the number and press enter: ";
            cin >> c;
            cin.ignore(); //ignore the enter key
            
            if(c == "1") {
                start_tutorial();
            } else if(c == "2") {
                start_game(PRACTICAL);
            } else if(c == "3") {
                start_game(NORMAL);
            } else if(c == "4") {
                settings_screen();
            } else if(c == "5") {
                load_screen();
            } else if(c == "6") {
                exit_screen();
                return 0;
            } else {
                cout << "INVALID INPUT" << endl;
                continue;
            }
            
            break;
        }
    }
}

void exit_screen() {
    cls();
    cout << endl;
    cout << "Have a nice day, Good bye =^_^=" << endl << endl;
}

void start_tutorial() {
    cls();
    cout << "TODO: TUTORIAL" << endl;
    readkey();
}

void print_board(vector<vector<Cell>> board) {
    printf("     ");
    
    for(int i = 0; i < board[0].size(); i++) {
        printf(" %c ", 'A' + i);
    }
    
    printf("\n");
    printf("\n");
    
    for(int i = 0; i < board.size(); i++) {
        printf("%2d   ", i + 1);
        
        for(int j = 0; j < board[i].size(); j++) {
            if(i == cur_i && j == cur_j) {
                if(board[i][j].status == WHITE) {
                    printf("\033[107;91m[%d]\033[0m", board[i][j].n);
                } else if(board[i][j].status == BLACK) {
                    printf("\033[40;91m[%d]\033[0m", board[i][j].n);
                } else {
                    printf("\033[100;91m[%d]\033[0m", board[i][j].n);
                }
            } else {
                if(board[i][j].status == WHITE) {
                    printf("\033[107;30m %d \033[0m", board[i][j].n);
                } else if(board[i][j].status == BLACK) {
                    printf("\033[40;37m %d \033[0m", board[i][j].n);
                } else {
                    printf("\033[100;30m %d \033[0m", board[i][j].n);
                }
            }
        }
        
        printf("\n");
//      printf("\033[0m\n");
    }
}

void start_game(GAME_MODE mode, vector<vector<Cell>>* savedBoard) {
    cls();
    string game_mode_str;
    
    if(mode == NORMAL) {
        game_mode_str = "Normal";
    } else {
        game_mode_str = "Practical";
    }
    
    cur_i = cur_j = 0;
    vector<vector<Cell>> board;
    
    if(savedBoard == NULL) {
        resetStacks();
        board = generate_board(board_size, debug_mode);
    } else {
        board = *savedBoard;
    }
    
    bool showHint = false;
    
    while(1) {
        cls();
        cout << "GAME MODE: " << game_mode_str << endl << endl;
        print_board(board);
        cout << endl;
        
        if(checkRows(board, showHint) && checkColumns(board, showHint) && checkWhite(board, showHint) && checkAdjBlackCell(board, showHint)) {
            cout << "YOU WIN!!!" << endl;
            cout << "Press any key to exit" << endl;
            readkey();
            return ;
        } else if(!showHint && mode == PRACTICAL) {
            checkWhite(board, true)&& checkAdjBlackCell(board, true);
        }
        
        showHint = false;
        cout << endl;
        cout << "\033[47;91mH A \033[30mLeft  \033[91mJ S \033[30mDown  \033[91mK W \033[30mUp  \033[91mL D \033[30mRight\033[91m                                           " << endl;
        cout << "(SPACE) \033[30mChange color  \033[91mE \033[30mExport  \033[91mU \033[30mUndo  \033[91mR \033[30mRedo  \033[91m? \033[30mHint  \033[91mX Q \033[30mExit                \033[0m" << endl;
        
        if(debug_mode) {
            printStack();
        }
        
        int c = readkey();
        
        if(c == 'x' || c == 'q') {      //Exit
            cout << endl << "Please press [Y] to confirm exit to main menu, or other keys to cancel" << endl;
            
            if(readkey() == 'y') {
                return;
            }
        } else if(c == '?') {           //Hint
            showHint = true;
        } else if(c == 'w' || c == 'k') { //UP
            if(cur_i > 0) {
                cur_i--;
            }
        } else if(c == 's' || c == 'j') { //DOWN
            if(cur_i < board.size() - 1) {
                cur_i++;
            }
        } else if(c == 'a' || c == 'h') { //LEFT
            if(cur_j > 0) {
                cur_j--;
            }
        } else if(c == 'd' || c == 'l') { //RIGHT
            if(cur_j < board[0].size() - 1) {
                cur_j++;
            }
        } else if(c == 'u') {       //Undo
            undo(board);
        } else if(c == 'r') {       //Redo
            redo(board);
        } else if(c == 'e') {       //Export
            exportGame(board, mode);
        } else if(c == ' ') {
            Cell e = board[cur_i][cur_j];
            board[cur_i][cur_j].status = e.status == UNMARKED ? WHITE : (e.status == WHITE ? BLACK : UNMARKED);
            Action* a = new Action();
            a->y = cur_i;
            a->x = cur_j;
            a->from = e.status;
            a->to = board[cur_i][cur_j].status;
            addAction(a);
        }
    }
}

void settings_screen() {
    cls();
    cout << "TODO: SETTINGS" << endl;
    //SET board_size
    //game save path
    readkey();
}

void load_screen() {
    cls();
    cout << "TODO: LOAD SAVED GAME" << endl;
    readkey();
}

void exportGame(vector<vector<Cell>> board, GAME_MODE mode) {
    string outfile_name = "board.sav";
    ofstream outfile;
    outfile.open(outfile_name, ios::out); //TODO: file name from config
    outfile << board_size << endl;
    outfile << mode << endl;
    
    for(int i = 0; i < board.size(); i++) {
        for(int j = 0; j < board[0].size(); j++) {
            outfile << board[i][j].n << " " << board[i][j].status << endl;
        }
    }
    
    outfile << endl;
    stack<Action*> tmp1(*(getUndoStack()));
    outfile << tmp1.size() << endl;
    
    while(tmp1.size() > 0) {
        outfile << actionToStr(tmp1.top());
        tmp1.pop();
    }
    
    stack<Action*> tmp2(*(getRedoStack()));
    outfile << tmp2.size() << endl;
    
    while(tmp2.size() > 0) {
        outfile << actionToStr(tmp2.top());
        tmp2.pop();
    }
    
    outfile.close();
    cout << "Saved to " << outfile_name << endl;
    cout << "Press any key to continue" << endl;
    readkey();
}
