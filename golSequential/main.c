#include "../util/dish.h"
#include "../util/golInput.h"


#define ADJACENT_NR 8
#define POS_X       0
#define POS_Y       1

size_t pos[ADJACENT_NR][2] = {
        {-1, -1}, { 0, -1}, { 1, -1},
        {-1,  0},           { 1,  0},
        {-1,  1}, { 0,  1}, { 1,  1}};

int toValidPos(int x, int n) {
    if (x < 0) return n - 1;
    if (x >= n) return 0;
    return x;
}

char updateCell(size_t x, size_t y, Dish dish, size_t n) {
    size_t nx, ny, aliveAdjacent = 0;
    for (int i = 0; i < ADJACENT_NR; ++i) {
        nx = toValidPos(x + pos[i][POS_X], n);
        ny = toValidPos(y + pos[i][POS_Y], n);
        aliveAdjacent += (dish[nx][ny] == ALIVE_CELL);
    }
    return ((dish[x][y] == ALIVE_CELL && (aliveAdjacent == 2 || aliveAdjacent == 3))
            || (dish[x][y] == DEAD_CELL && aliveAdjacent == 3)) ? ALIVE_CELL : DEAD_CELL;
}

void updateDish(Dish prevDish, Dish currDish, size_t n) {
    for (size_t r = 0; r < n; ++r) {
        for (size_t c = 0; c < n; ++c) {
            currDish[r][c] = updateCell(r, c, prevDish, n);
        }
    }
}


int main() {
    size_t n, it;
    Dish currDish = readDish(&n),
            prevDish = createDish(n, n),
            tmp;

    scanf("%lu", &it);
    for (size_t i = 0; i < it; ++i) {
        tmp = currDish;
        currDish = prevDish;
        prevDish = tmp;
        updateDish(prevDish, currDish, n);
    }

    printDish(currDish, n, n);
    freeDish(prevDish, n);
    freeDish(currDish, n);
}
