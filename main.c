#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include "game.h"

// カーソル位置の設定
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 画面のクリア
void clearScreen() {
    system("cls");
}

// メニュー表示
void showMenu(int selected) {
    clearScreen();
    printf("オセロゲーム\n");
    printf("%s 1: VS モード\n", selected == 0 ? "→" : "  ");
    printf("%s 2: CPU モード\n", selected == 1 ? "→" : "  ");
    printf("%s 3: 終了\n", selected == 2 ? "→" : "  ");
}

// 盤面表示
void showBoard() {
    clearScreen();
    printf("  0 1 2 3 4 5 6 7\n");
    for (int y = 0; y < 8; y++) {
        printf("%d ", y);
        for (int x = 0; x < 8; x++) {
            printf("%c ", field.boardState.board[y][x]);
        }
        printf("\n");
    }
    printf("\n");
    printf("黒: %d, 白: %d\n", field.boardState.blackCount, field.boardState.whiteCount);
    printf("%sの番です。\n", field.Is_A_Turn ? "黒" : "白");
    printf("ESC: メニューに戻る\n");
}

int main() {
    // コンソールの設定
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    // ゲームの初期化
    Init();

    // メインループ
    while (1) {
        switch (field.step_mode) {
            case Menu: {
                int selected = 0;
                showMenu(selected);

                while (1) {
                    int key = _getch();
                    if (key == 224) {  // 矢印キー
                        key = _getch();
                        switch (key) {
                            case 72:  // 上
                                selected = (selected - 1 + 3) % 3;
                                showMenu(selected);
                                break;
                            case 80:  // 下
                                selected = (selected + 1) % 3;
                                showMenu(selected);
                                break;
                        }
                    }
                    else if (key == 13) {  // Enter
                        switch (selected) {
                            case 0:
                                field.step_mode = VS;
                                goto game_start;
                            case 1:
                                field.step_mode = CPU;
                                goto game_start;
                            case 2:
                                clearScreen();
                                printf("ゲームを終了します。\n");
                                printf("Enterキーを押してください...");
                                while (_getch() != 13);
                                return 0;
                        }
                    }
                }
                break;
            }

            case VS: {
game_start:
                // ゲームの初期化
                Init();
                showBoard();
                int cursorX = 0, cursorY = 0;
                setCursorPosition(cursorX * 2 + 2, cursorY + 1);

                while (1) {
                    int key = _getch();
                    if (key == 224) {  // 矢印キー
                        key = _getch();
                        int oldX = cursorX;
                        int oldY = cursorY;
                        switch (key) {
                            case 72:  // 上
                                if (cursorY > 0) cursorY--;
                                break;
                            case 80:  // 下
                                if (cursorY < 7) cursorY++;
                                break;
                            case 75:  // 左
                                if (cursorX > 0) cursorX--;
                                break;
                            case 77:  // 右
                                if (cursorX < 7) cursorX++;
                                break;
                        }
                        // カーソル位置を更新
                        setCursorPosition(cursorX * 2 + 2, cursorY + 1);
                    }
                    else if (key == 13) {  // Enter
                        if (placeStone(&field.boardState, cursorX, cursorY, field.Is_A_Turn)) {
                            field.Is_A_Turn = !field.Is_A_Turn;
                            field.turn++;
                            showBoard();
                            setCursorPosition(cursorX * 2 + 2, cursorY + 1);
                        }
                    }
                    else if (key == 27) {  // ESC
                        field.step_mode = Menu;
                        break;
                    }

                    // ゲーム終了チェック
                    if (checkGameEnd(&field.boardState)) {
                        showBoard();
                        printf("\nゲーム終了\n");
                        printf("黒: %d, 白: %d\n", field.boardState.blackCount, field.boardState.whiteCount);
                        if (field.boardState.blackCount > field.boardState.whiteCount) {
                            printf("黒の勝ちです。\n");
                        } else if (field.boardState.blackCount < field.boardState.whiteCount) {
                            printf("白の勝ちです。\n");
                        } else {
                            printf("引き分けです。\n");
                        }
                        printf("Enterキーを押してください...");
                        while (_getch() != 13);
                        field.step_mode = Menu;
                        break;
                    }
                }
                break;
            }

            case CPU: {
                // ゲームの初期化
                Init();
                showBoard();
                int cursorX = 0, cursorY = 0;
                setCursorPosition(cursorX * 2 + 2, cursorY + 1);

                while (1) {
                    if (field.Is_A_Turn) {
                        int key = _getch();
                        if (key == 224) {  // 矢印キー
                            key = _getch();
                            int oldX = cursorX;
                            int oldY = cursorY;
                            switch (key) {
                                case 72:  // 上
                                    if (cursorY > 0) cursorY--;
                                    break;
                                case 80:  // 下
                                    if (cursorY < 7) cursorY++;
                                    break;
                                case 75:  // 左
                                    if (cursorX > 0) cursorX--;
                                    break;
                                case 77:  // 右
                                    if (cursorX < 7) cursorX++;
                                    break;
                            }
                            // カーソル位置を更新
                            setCursorPosition(cursorX * 2 + 2, cursorY + 1);
                        }
                        else if (key == 13) {  // Enter
                            if (placeStone(&field.boardState, cursorX, cursorY, true)) {
                                field.Is_A_Turn = false;
                                field.turn++;
                                showBoard();
                                setCursorPosition(cursorX * 2 + 2, cursorY + 1);
                            }
                        }
                        else if (key == 27) {  // ESC
                            field.step_mode = Menu;
                            break;
                        }
                    } else {
                        // CPUの手
                        decideCPUMove(&field.boardState, false, field.cpu_type);
                        field.Is_A_Turn = true;
                        field.turn++;
                        showBoard();
                        setCursorPosition(cursorX * 2 + 2, cursorY + 1);
                    }

                    // ゲーム終了チェック
                    if (checkGameEnd(&field.boardState)) {
                        showBoard();
                        printf("\nゲーム終了\n");
                        printf("黒: %d, 白: %d\n", field.boardState.blackCount, field.boardState.whiteCount);
                        if (field.boardState.blackCount > field.boardState.whiteCount) {
                            printf("プレイヤーの勝ちです。\n");
                        } else if (field.boardState.blackCount < field.boardState.whiteCount) {
                            printf("CPUの勝ちです。\n");
                        } else {
                            printf("引き分けです。\n");
                        }
                        printf("Enterキーを押してください...");
                        while (_getch() != 13);
                        field.step_mode = Menu;
                        break;
                    }
                }
                break;
            }
        }
    }

    return 0;
} 