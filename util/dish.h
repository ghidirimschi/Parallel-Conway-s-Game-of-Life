#ifndef GOLMPI_DISH_H
#define GOLMPI_DISH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef char** Dish;

Dish createSubDish(size_t height, size_t width);
void freeSubDish(Dish subDish, size_t height, size_t width);

Dish createDish(size_t height, size_t width);
void freeDish(Dish dish, size_t n);

void printDish(Dish dish, size_t height, size_t width);

#endif //GOLMPI_DISH_H
