#include <stdio.h>
#include "utility.h"

void printLocation(Point* point) {
    printf("X座標は%d、Y座標は%dです。\n", point->x, point->y);
}

void move(Point* point, int x, int y) {
    point->x = x;
    point->y = y;
}
