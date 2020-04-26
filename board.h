#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
using namespace std;

bool checkWhiteConnected(vector<vector<Cell>> board);
void blackTheCell(vector<vector<Cell>>& board, int i, int j);
vector<vector<Cell>> generate_board(int w, bool debug_mode = false);
bool checkRows(vector<vector<Cell>> borad, bool out = true);
bool checkColumns(vector<vector<Cell>> board, bool out = true);
bool checkWhite(vector<vector<Cell>> board, bool out = true);
bool checkAdjBlackCell(vector<vector<Cell>> board, bool out = true);

#endif