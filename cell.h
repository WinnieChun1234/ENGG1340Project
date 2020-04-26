#ifndef CELL_H
#define CELL_H

#include <vector>
using namespace std;

enum cell_status { UNMARKED = 0, WHITE = 1, BLACK = 2 };

typedef struct {
    int n;
    cell_status status;
} Cell;

void swap(vector<Cell>& a, vector<Cell>& b);

#endif