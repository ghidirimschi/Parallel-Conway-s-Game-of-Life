#include "dish.h"

Dish createSubDish(size_t height, size_t width) {
    Dish dish = malloc((height + 2) * sizeof(char *));
    dish[0] = malloc(width * sizeof(char));
    dish[1] = malloc(height * width * sizeof(char));
    for (size_t i = 2; i < height + 1; ++i) {
        dish[i] = dish[i - 1] + width;
    }
    dish[height + 1] = malloc(width * sizeof(char));
    return dish;
}

void freeSubDish(Dish subDish, size_t height, size_t width) {
    free(subDish[0]);
    free(subDish[1]);
    free(subDish[height + 1]);
    free(subDish);
}

Dish createDish(size_t height, size_t width) {
    Dish dish = malloc(height * sizeof(char *));
    dish[0] = malloc(height * width * sizeof(char));
    for (size_t i = 1; i < height; ++i) {
        dish[i] = dish[i - 1] + width;
    }
    return dish;
}

void freeDish(Dish dish, size_t n) {
    free(dish[0]);
    free(dish);
}

void printDish(Dish dish, size_t height, size_t width) {
    for (size_t r = 0; r < height; ++r) {
        for (size_t c = 0; c < width; ++c) {
            printf("%c", dish[r][c]);
        }
        printf("\n");
    }
}
