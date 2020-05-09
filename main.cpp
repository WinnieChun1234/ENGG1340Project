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

////////BELOW FUNCTION MODIFIED FROM https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar
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
////////ABOVE FUNCTION MODIFIED FROM https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar

enum GAME_MODE { PRACTICE = 1, NORMAL = 2 };
void start_tutorial();
void start_game(GAME_MODE, vector<vector<Cell>>* savedBoard = NULL);
void settings_screen();
void load_screen();
void exit_screen();
void exportGame(vector<vector<Cell>> board, GAME_MODE mode);
void print_board(vector<vector<Cell>>);

void cls() { //clear screen
    cout << "\033[2J\033[1;1H";
}

int board_size = 6;
int cur_i, cur_j;
bool debug_mode = false;
string save_path = "board.sav";

int main(int argc, char* argv[]) {
    //debug mode will be enabled if the program was launched by "./main -d"
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
        cout << "2. Practice Mode" << endl;
        cout << "3. Normal Mode" << endl;
        cout << "4. Settings" << endl;
        cout << "5. Load Saved Game" << endl;
        cout << "6. Exit" << endl;
        cout << endl;

        while(1) { //keep looping until getting the correct input, and go to the destination screen
            cout << "Please enter the number and press enter: ";
            cin >> c;
            cin.ignore(); //ignore the enter key, to prevent the enter key mistakenly read by the readkey() function

            if(c == "1") {
                start_tutorial();
            } else if(c == "2") {
                start_game(PRACTICE);
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

//exit screen
void exit_screen() {
    cls();
    cout << endl;
    cout << "Have a nice day, Good bye =^_^=" << endl << endl;
}

//tutorial mode
void start_tutorial() {
    cls();
    //create preset board
    int presetBoard[6][6] = {5, 6, 5, 1, 2, 2,
                             3, 2, 4, 5, 1, 3,
                             2, 3, 2, 6, 6, 4,
                             3, 4, 5, 6, 3, 6,
                             4, 5, 4, 3, 2, 1,
                             3, 4, 2, 4, 4, 5};
    vector<vector<Cell>> board(6);

    for(int i = 0; i < 6; i++) {
        vector<Cell> m(6);

        for(int j = 0; j < 6; j++) {
            m[j].n = presetBoard[j][i];
            m[j].status = UNMARKED;
        }

        board[i] = m;
    }

    int pageNumber = 0;
    bool isFinished = false;

    while(!isFinished) {
        cls();
        string temp = "";

        switch(pageNumber) {
        case 0:
            temp += "The goal of the game is to blacken some cells\n";
            temp += "such that the following rules are not violated:\n";
            temp += "1.  No two cells should be blackened in adjacent,\n";
            temp += "    either horizontally or vertically.\n";
            temp += "2.  No two cells with the same number in the same\n";
            temp += "    row/column should be whitened.\n";
            temp += "3.  All whitened cells should be connected horizontally\n";
            temp += "    or vertically with one another.\n";
            temp += "\n";
            temp += "This tutorial will provide some basic solving techniques,\n";
            temp += "derived from the above rules. Let's get started! ^v^\n";
            break;

        case 1:
            temp += "- Search for a cell in between cells with same numbers\n";
            temp += "As the two outer cells cannot be both white (Rule 2),\n";
            temp += "at least one of them should be black. Therefore,\n";
            temp += "the middle cell should be marked white (Rule 1).\n";

            for(int n : {22}) {
                board[n % 6][n / 6].status = WHITE;
            }
            cur_i = 22 % 6;
            cur_j = 22 / 6;
            break;

        case 2:
            temp += "- Search for a cell in between cells with same numbers\n";
            temp += "These are all the cells that matches this pattern.\n";

            for(int n : {1, 12, 13, 22, 25, 32}) {
                board[n % 6][n / 6].status = WHITE;
            }
            break;

        case 3:
            temp += "- Blacken cells in the same row/column as a white cell with the same number\n";
            temp += "This technique is derived from Rule 2.\n";

            for(int n : {14}) {
                board[n % 6][n / 6].status = BLACK;
            }
            cur_i = 14 % 6;
            cur_j = 14 / 6;
            break;

        case 4:
            temp += "- Blacken cells in the same row/column as a white cell with the same number\n";
            temp += "These are all the cells that matches this pattern.\n";

            for(int n : {14, 18}) {
                board[n % 6][n / 6].status = BLACK;
            }
            break;

        case 5:
            temp += "- Whiten cells adjacent to a black cell\n";
            temp += "This technique is derived from Rule 1.\n";

            for(int n : {15}) {
                board[n % 6][n / 6].status = WHITE;
            }
            cur_i = 15 % 6;
            cur_j = 15 / 6;
            break;

        case 6:
            temp += "- Whiten cells adjacent to a black cell\n";
            temp += "These are all the cells that matches this pattern.\n";

            for(int n : {8, 15, 19, 20, 24}) {
                board[n % 6][n / 6].status = WHITE;
            }
            break;

        case 7:
            temp += "- Blacken cells in the same row/column as a white cell with the same number\n";
            temp += "- Whiten cells adjacent to a black cell\n";
            temp += "These two techniques will be the most likely to be used.\n";
            temp += "Check frequently to see if these can be applied.\n";

            for(int n : {0, 3, 7, 10, 17, 27, 30}) {
                board[n % 6][n / 6].status = WHITE;
            }

            for(int n : {2, 6, 16, 21, 26, 31}) {
                board[n % 6][n / 6].status = BLACK;
            }
            break;

        case 8:
            temp += "- Whiten cell that will separate other white cells in two otherwise\n";
            temp += "This technique is derived from Rule 3.\n";

            for(int n : {33}) {
                board[n % 6][n / 6].status = WHITE;
            }
            cur_i = 33 % 6;
            cur_j = 33 / 6;
            break;

        case 9:
            temp += "- Whiten cell that will separate other white cells in two otherwise\n";
            temp += "These are all the cells that matches this pattern.\n";

            for(int n : {9, 11, 23, 33}) {
                board[n % 6][n / 6].status = WHITE;
            }
            break;

        case 10:
            temp += "There are other techniques to be discovered.\n";
            temp += "Try to come up one on your own! ;)\n";
            temp += "The above board is in a solved state. There may be more than\n";
            temp += "one solved states, but any one of them will be reconized as a solution.\n";
            temp += "\n";
            temp += "Have fun playing Solitudo! ^o^\n";

            for(int n : {5, 28, 29, 35}) {
                board[n % 6][n / 6].status = WHITE;
            }

            for(int n : {4, 34}) {
                board[n % 6][n / 6].status = BLACK;
            }

        default:
            isFinished = true;
        }

        cout << "TUTORIAL MODE" << endl << endl;

        if(pageNumber != 0) {
            print_board(board);
        }

        cout << endl;
        cout << temp << endl;
        pageNumber++;
        cout << "Press any key to continue." << endl;
        readkey();
    }

    return;
}


//print the given board onto the screen
void print_board(vector<vector<Cell>> board) {
    printf("     ");

    //display column label
    for(int i = 0; i < board[0].size(); i++) {
        printf(" %c ", 'A' + i);
    }

    printf("\n");
    printf("\n");

    for(int i = 0; i < board.size(); i++) {//for each row
        printf("%2d   ", i + 1);    //display row label

        for(int j = 0; j < board[i].size(); j++) {//for each cell
            if(i == cur_i && j == cur_j) { //if the cursor is in this cell, show the number with square bracket, e.g. [1]
                if(board[i][j].status == WHITE) {
                    printf("\033[107;91m[%d]\033[0m", board[i][j].n);   //display cell in white background
                } else if(board[i][j].status == BLACK) {
                    printf("\033[40;91m[%d]\033[0m", board[i][j].n);    //display cell in black background
                } else {
                    printf("\033[100;91m[%d]\033[0m", board[i][j].n);   //display cell in gray background for "UNMARKED" cell
                }
            } else {
                if(board[i][j].status == WHITE) {
                    printf("\033[107;30m %d \033[0m", board[i][j].n);   //display cell in white background
                } else if(board[i][j].status == BLACK) {
                    printf("\033[40;37m %d \033[0m", board[i][j].n);    //display cell in black background
                } else {
                    printf("\033[100;30m %d \033[0m", board[i][j].n);   //display cell in gray background for "UNMARKED" cell
                }
            }
        }

        printf("\n");
    }
}

//start the game in with the game in specified mode with a saved board (if any)
void start_game(GAME_MODE mode, vector<vector<Cell>>* savedBoard) {
    cls();
    string game_mode_str;

    if(mode == NORMAL) {
        game_mode_str = "Normal";
    } else {
        game_mode_str = "Practice";
    }

    cur_i = cur_j = 0;
    vector<vector<Cell>> board;

    if(savedBoard == NULL) {    //generate a new board if there are no saved board
        resetStacks();
        board = generate_board(board_size, debug_mode);
    } else {
        //DO NOT reset stacks if saved board is used
        board = *savedBoard;
    }

    bool showHint = false;

    while(1) {
        cls();
        cout << "GAME MODE: " << game_mode_str << endl << endl;
        print_board(board);
        cout << endl;

        //check if user win, if user asked for hint, display also the result from each checking
        if(checkRows(board, showHint) && checkColumns(board, showHint) && checkWhite(board, showHint) && checkAdjBlackCell(board, showHint)) {
            cout << "YOU WIN!!!" << endl;
            cout << "Press any key to exit" << endl;
            readkey();
            return ;
        } else if(!showHint && mode == PRACTICE) {
            //for practice mode, check if all white cell connected, if yes, check also if there are any adjacent black cell
            checkWhite(board, true)&& checkAdjBlackCell(board, true);
        }

        showHint = false;
        //display the key list
        cout << endl;
        cout << "\033[47;91mH A \033[30mLeft  \033[91mJ S \033[30mDown  \033[91mK W \033[30mUp  \033[91mL D \033[30mRight\033[91m                                           " << endl;
        cout << "(SPACE) \033[30mChange color  \033[91mE \033[30mExport  \033[91mU \033[30mUndo  \033[91mR \033[30mRedo  \033[91m? \033[30mHint  \033[91mX Q \033[30mExit                \033[0m" << endl;

        //print undo/redo stack if debug mode enabled
        if(debug_mode) {
            printStack();
        }

        //read user input
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
        } else if(c == ' ') {       //Change color
            //change the color of current cell
            Cell e = board[cur_i][cur_j];   //save a copy of the current cell
            board[cur_i][cur_j].status = e.status == UNMARKED ? WHITE : (e.status == WHITE ? BLACK : UNMARKED); //change the status
            //create a new action and assign value into it
            Action* a = new Action();
            a->y = cur_i;
            a->x = cur_j;
            a->from = e.status;
            a->to = board[cur_i][cur_j].status;
            //add the action to the undo stack. (note: this function will also clear the redo stack)
            addAction(a);
        }
    }
}

void settings_screen() {
    cls();
    cout << "Current board size: " << board_size << endl;
    cout << "New board size : ";
    cin >> board_size;

    while(board_size < 5 || board_size > 9) {
        cout << "The board size must be between 5 and 9." << endl;
        cout << "Current board size: " << board_size << endl;
        cout << "New board size : ";
        cin >> board_size;
    }

    cout << endl;
    cout << "Current game save path: " << save_path << endl;
    cout << "New game save path: ";
    cin >> save_path;

    while(true) {
        ofstream test_file(save_path, fstream::in | fstream::out | fstream::app);

        if(test_file.is_open()) {
            test_file.close();
            break;
        }

        cout << "Invalid path" << endl;
        cout << "Current game save path: " << save_path << endl;
        cout << "New game save path: ";
        cin >> save_path;
    }

    cout << "Press any key to continue" << endl;
    readkey();
}

//load saved game from file. (refer to exportGame() for the file format)
void load_screen() {
    cls();
    string infile_name = save_path;
    ifstream infile;
    infile.open(infile_name, ios::in);
    GAME_MODE mode;
    int tmp;
    infile >> board_size;   //get the board size
    //cout << "Board Size:    " << board_size << endl;
    infile >> tmp;  //get the game mode
    mode = GAME_MODE(tmp);
    //cout << "Game Mode:     " << mode << endl;
    int w = board_size;
    int status_int = 0;
    vector<vector<Cell>> board(w);

    //set the board
    for(int i = 0; i < w; i++) {
        board[i] = vector<Cell>(w);

        for(int j = 0; j < w; j++) {
            //cout << "\rLoading board [" << i << "," << j << "]";
            infile >> board[i][j].n >> status_int;
            board[i][j].status = cell_status(status_int);
        }
    }

    cout << endl;
    resetStacks();  //reset the undo/redo stack
    int n;
    infile >> n;    //get number of action in undo stack
    infile.ignore();    //ignore the \n
    //cout << "Undo Stack Size: " << n << endl;
    string s;
    stack<Action*> tmp1;

    //add undo action into a temperary stack
    for(int i = 0; i < n; i++) {
        getline(infile, s);
        //cout << "Loading action "<<s<<endl;
        tmp1.push(stringToAction(s));
    }

    //as undo action added into tmp1 is upside-down, pop and push into the actual undo stack
    while(tmp1.size() > 0) {
        getUndoStack()->push(tmp1.top());
        tmp1.pop();
    }

    infile >> n;    //get number of action in redo stack
    infile.ignore();    //ignore the \n
    //cout << "Redo Stack Size: " << n << endl;
    stack<Action*> tmp2;

    //add redo action into a temperary stack
    for(int i = 0; i < n; i++) {
        getline(infile, s);
        //cout << "Loading action "<<s<<endl;
        tmp2.push(stringToAction(s));
    }

    //as redo action added into tmp1 is upside-down, pop and push into the actual undo stack
    while(tmp2.size() > 0) {
        getRedoStack()->push(tmp2.top());
        tmp2.pop();
    }

    infile.close();
    cout << "Loaded the file " << infile_name << ". Press any key to continue." << endl;
    cin.ignore();
    readkey();
    start_game(mode, &board);
}

void exportGame(vector<vector<Cell>> board, GAME_MODE mode) {
    string outfile_name = save_path;
    ofstream outfile;
    outfile.open(outfile_name, ios::out);
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
