#include "board.h"
#include <string.h>

// オセロ盤のセルの描画
void drawCell(char* buffer, int x, int y, char cell) {
    int startX = BOARD_START_X + x * CELL_WIDTH;
    int startY = BOARD_START_Y + y * CELL_HEIGHT;

    // セルの背景
    for (int i = 0; i < CELL_HEIGHT; i++) {
        for (int j = 0; j < CELL_WIDTH; j++) {
            buffer[(startY + i) * CONSOLE_WIDTH + startX + j] = ' ';
        }
    }

    // 石の描画
    if (cell != ' ') {
        char stone = cell;
        buffer[(startY + CELL_HEIGHT/2) * CONSOLE_WIDTH + startX + CELL_WIDTH/2] = stone;
    }
}

// オセロ盤の枠の描画
void drawBoardFrame(char* buffer) {
    // 横線
    for (int y = 0; y <= BOARD_SIZE; y++) {
        int startY = BOARD_START_Y + y * CELL_HEIGHT;
        for (int x = 0; x <= BOARD_SIZE * CELL_WIDTH; x++) {
            buffer[startY * CONSOLE_WIDTH + BOARD_START_X + x] = '-';
        }
    }

    // 縦線
    for (int x = 0; x <= BOARD_SIZE; x++) {
        int startX = BOARD_START_X + x * CELL_WIDTH;
        for (int y = 0; y <= BOARD_SIZE * CELL_HEIGHT; y++) {
            buffer[(BOARD_START_Y + y) * CONSOLE_WIDTH + startX] = '|';
        }
    }

    // 交点
    for (int y = 0; y <= BOARD_SIZE; y++) {
        for (int x = 0; x <= BOARD_SIZE; x++) {
            buffer[(BOARD_START_Y + y * CELL_HEIGHT) * CONSOLE_WIDTH + 
                   BOARD_START_X + x * CELL_WIDTH] = '+';
        }
    }
}

// ゲーム情報の描画
void drawGameInfo(char* buffer, const field_game* game) {
    char info[32];
    
    // ターン情報
    snprintf(info, sizeof(info), "Turn: %d", game->turn);
    memcpy(buffer + 1 * CONSOLE_WIDTH + 2, info, strlen(info));

    // プレイヤー情報
    const char* player = game->Is_A_Turn ? "Player A (黒)" : "Player B (白)";
    memcpy(buffer + 2 * CONSOLE_WIDTH + 2, player, strlen(player));

    // ゲームモード
    const char* mode = game->step_mode == VS ? "VS Mode" : "CPU Mode";
    memcpy(buffer + 3 * CONSOLE_WIDTH + 2, mode, strlen(mode));

    // 石の数
    snprintf(info, sizeof(info), "黒: %d  白: %d", 
             game->boardState.blackCount, 
             game->boardState.whiteCount);
    memcpy(buffer + 4 * CONSOLE_WIDTH + 2, info, strlen(info));
}

// カーソルの描画
void drawCursor(char* buffer, int x, int y) {
    int startX = BOARD_START_X + x * CELL_WIDTH;
    int startY = BOARD_START_Y + y * CELL_HEIGHT;

    // カーソルの表示
    buffer[startY * CONSOLE_WIDTH + startX] = '[';
    buffer[startY * CONSOLE_WIDTH + startX + CELL_WIDTH - 1] = ']';
}

// オセロ盤の描画
void drawBoard(char* buffer, const field_game* game) {
    // 画面クリア
    memset(buffer, ' ', CONSOLE_WIDTH * CONSOLE_HEIGHT);

    // ゲーム情報の描画
    drawGameInfo(buffer, game);

    // オセロ盤の枠の描画
    drawBoardFrame(buffer);

    // 石の描画
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            char cell = game->boardState.board[y][x];
            drawCell(buffer, x, y, cell);
        }
    }

    // 置ける場所の表示
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (game->boardState.canPlace[y][x]) {
                int startX = BOARD_START_X + x * CELL_WIDTH;
                int startY = BOARD_START_Y + y * CELL_HEIGHT;
                buffer[(startY + CELL_HEIGHT/2) * CONSOLE_WIDTH + startX + CELL_WIDTH/2] = '.';
            }
        }
    }
}
