#ifndef GOLMPI_GOLINPUT_H
#define GOLMPI_GOLINPUT_H

#include "dish.h"

#define ALIVE_CELL   '#'
#define DEAD_CELL    '.'

void removeNewLine(char *s, size_t *len);
void checkValidSymbol(size_t row, size_t col, char symbol);
Dish readDish(size_t *size);

#endif //GOLMPI_GOLINPUT_H
