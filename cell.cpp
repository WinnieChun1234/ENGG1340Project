#include <vector>
#include "cell.h"

//to swap two vectors of cells
void swap(vector<Cell>& a, vector<Cell>& b) {
    vector<Cell> tmp(a);
    a = b;
    b = tmp;
}
