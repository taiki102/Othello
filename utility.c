#include <stdio.h>
#include "utility.h"

Vec2Int Plus(Vec2Int x, Vec2Int y) {
    x.x += y.x;
    x.y += y.y;
    return x;
}

const Vec2Int dir[] = {
    {0, 1},    // ��
    {1, 1},    // �E��i�΂߁j
    {1, 0},    // �E
    {1, -1},   // �E���i�΂߁j
    {0, -1},   // ��
    {-1, -1},  // �����i�΂߁j
    {-1, 0},   // ��
    {-1, 1}    // ����i�΂߁j
};