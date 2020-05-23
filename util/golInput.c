#define _GNU_SOURCE
#include "golInput.h"

void removeNewLine(char *s, size_t *len) {
    if (s[*len - 1] == '\n') {
        s[*len - 1] = '\0';
        *len = *len - 1;
    }
}

void checkValidSymbol(size_t row, size_t col, char symbol) {
    if (symbol != ALIVE_CELL && symbol != DEAD_CELL) {
        fprintf(stderr, "Error on (%lu, %lu): %c is invalid symbol (use %c for alive or %c for dead cell)\n", row, col, symbol, ALIVE_CELL, DEAD_CELL);
        exit(EXIT_FAILURE);
    }
}

Dish readDish(size_t *size) {
    char *line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);

    size_t n = strlen(line);
    removeNewLine(line, &n);

    for (size_t i = 0; i < n; ++i) {
        checkValidSymbol(0, i, line[i]);
    }

    Dish dish = malloc(n * sizeof(char *));
    line = realloc(line, n * n * sizeof(char));
    dish[0] = line;
    for (size_t r = 1; r < n; ++r) {
        dish[r] = dish[r - 1] + n;
        for (size_t c = 0; c < n; ++c) {
            dish[r][c] = getchar();
            checkValidSymbol(r, c, dish[r][c]);
        }
        if (getchar() != '\n') {
            fprintf(stderr, "Error on row %lu: Width must be %lu\n", r, n);
            exit(EXIT_FAILURE);
        }
    }
    *size = n;
    return dish;
}