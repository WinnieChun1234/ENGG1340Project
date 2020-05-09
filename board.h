#ifndef BOARD_H
#define BOARD_H

#include "cell.h"
using namespace std;

bool check_white_connected(vector<vector<Cell>> board);
void black_the_cell(vector<vector<Cell>>& board, int i, int j);
vector<vector<Cell>> generate_board(int w, bool debug_mode = false);
bool check_rows(vector<vector<Cell>> borad, bool out = true);
bool check_columns(vector<vector<Cell>> board, bool out = true);
bool check_white(vector<vector<Cell>> board, bool out = true);
bool check_adj_black_cell(vector<vector<Cell>> board, bool out = true);

#endif