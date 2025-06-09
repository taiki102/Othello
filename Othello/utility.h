#pragma once
#ifndef POINT_H
#define POINT_H

typedef struct Point {
    int x;    // Xç¿ïW
    int y;    // Yç¿ïW
} Point;

void printLocation(Point*);
void move(Point*, int, int);

#endif 