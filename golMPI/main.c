#include "../util/dish.h"
#include "../util/golInput.h"
#include <mpi.h>

int rank, size;

#define ADJACENT_NR 8
#define POS_X       0
#define POS_Y       1


/**
 * The relative positions of adjacent cells.
 */
size_t pos[ADJACENT_NR][2] = {
        {-1, -1}, { 0, -1}, { 1, -1},
        {-1,  0},           { 1,  0},
        {-1,  1}, { 0,  1}, { 1,  1}};


/**
 * Given a value r in the range [-1, s+1), return r mod s. That is,
 * if r is equal to s return 0, if r is -1, return s -1 and r otherwise.
 */
int mod(int r, int s) {
    if (r < 0) {
        return s - 1;
    }
    if (r >= s) {
        return 0;
    }
    return r;
}

char updateCell(size_t x, size_t y, Dish dish, size_t width) {
    size_t nx, ny, aliveAdjacent = 0;
    for (int i = 0; i < ADJACENT_NR; ++i) {
        nx = x + pos[i][POS_X];
        ny = mod(y + pos[i][POS_Y], width);
        aliveAdjacent += (dish[nx][ny] == ALIVE_CELL);
    }
    return ((dish[x][y] == ALIVE_CELL && (aliveAdjacent == 2 || aliveAdjacent == 3))
            || (dish[x][y] == DEAD_CELL && aliveAdjacent == 3)) ? ALIVE_CELL : DEAD_CELL;
}


int updateBoundaries(Dish subDish, const int *rows, int width) {
    MPI_Status status;
    MPI_Sendrecv(subDish[rows[rank]], width, MPI_CHAR,
                 mod(rank + 1, size), 0, subDish[0], width, MPI_CHAR,
                 mod((int)rank - 1, size), 0, MPI_COMM_WORLD, &status);
    MPI_Sendrecv(subDish[1], width, MPI_CHAR,
                 mod((int)rank - 1, size), 0, subDish[rows[rank] + 1],
                 width, MPI_CHAR,mod(rank + 1, size), 0, MPI_COMM_WORLD, &status);

}

void updateSubDish(Dish prevSubDish, Dish currSubDish, int height, int width) {
    for (size_t r = 1; r <= height; ++r) {
        for (size_t c = 0; c < width; ++c) {
            currSubDish[r][c] = updateCell(r, c, prevSubDish, width);
        }
    }
}


int *distributeRows(size_t n) {
    int rest = n % size, quotient = n / size,
            *rows = malloc(size * sizeof(int));

    for (size_t i = 0; i < size; ++i) {
        rows[i] = quotient + (i < rest);
    }
    return rows;
}

int *computeDispls(const int *rows, int n) {
    int *displs = malloc(size * sizeof(int));
    displs[0] = 0;
    for (size_t i = 1; i < size; ++i) {
        displs[i] = displs[i - 1] + n * rows[i - 1];
    }
    return displs;

}

int *computeSendcounts(const int *rows, int n) {
    int *sendcounts = malloc(size * sizeof(int));
    for (size_t i = 0; i < size; ++i) {
        sendcounts[i] = rows[i] * n;
    }
    return sendcounts;
}

int main (int argc, char **argv) {
    MPI_Init (&argc, &argv);
    MPI_Comm_rank (MPI_COMM_WORLD, &rank);
    MPI_Comm_size (MPI_COMM_WORLD, &size);

    size_t n, it;
    Dish dish = NULL;

    if (!rank) {
        dish = readDish(&n);
        scanf("%lu\n", &it);
    }

    MPI_Bcast(&n, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);
    MPI_Bcast(&it, 1, MPI_UNSIGNED_LONG, 0, MPI_COMM_WORLD);

    int *rows = distributeRows(n);

    Dish currSubDish = createSubDish(rows[rank], n),
         prevSubDish = createSubDish(rows[rank], n),
         tmp;

    int *sendcounts = computeSendcounts(rows, n),
            *displs = computeDispls(rows, n);

    MPI_Scatterv(rank ? NULL : dish[0], sendcounts, displs, MPI_CHAR, currSubDish[1],
                 sendcounts[rank] + n, MPI_CHAR, 0, MPI_COMM_WORLD);

    for (size_t i = 0; i < it; ++i) {
        tmp = prevSubDish;
        prevSubDish = currSubDish;
        currSubDish = tmp;
        updateBoundaries(prevSubDish, rows, n);
        updateSubDish(prevSubDish, currSubDish, rows[rank], n);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Gatherv(currSubDish[1], sendcounts[rank], MPI_CHAR,
                rank ? NULL : dish[0], sendcounts, displs,
                MPI_CHAR, 0, MPI_COMM_WORLD);

    if (!rank) {
        printDish(dish, n, n);
        freeDish(dish, n);
    }

    freeSubDish(prevSubDish, rows[rank], n);
    freeSubDish(currSubDish, rows[rank], n);
    free(rows);
    free(sendcounts);
    free(displs);


    MPI_Finalize ();
    return EXIT_SUCCESS;

}

