#include <stdio.h>
#include "utility.h"

void printLocation(Point* point) {
    printf("X���W��%d�AY���W��%d�ł��B\n", point->x, point->y);
}

void move(Point* point, int x, int y) {
    point->x = x;
    point->y = y;
}
