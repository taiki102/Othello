#ifndef BOARD_H
#define BOARD_H

#include "game.h"
#include "consoleCtl.h"

// オセロ盤の描画設定
#define BOARD_SIZE 8
#define BOARD_START_X 20
#define BOARD_START_Y 5
#define CELL_WIDTH 4
#define CELL_HEIGHT 2

// オセロ盤の描画関数
void drawBoard(char* buffer, const field_game* game);

// オセロ盤のセルの描画
void drawCell(char* buffer, int x, int y, char cell);

// オセロ盤の枠の描画
void drawBoardFrame(char* buffer);

// ゲーム情報の描画
void drawGameInfo(char* buffer, const field_game* game);

// カーソルの描画
void drawCursor(char* buffer, int x, int y);

#endif // BOARD_H



