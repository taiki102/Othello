#include <stdio.h>
#include "game.h"
#include "consoleCtl.h"
#include <windows.h>
#include <conio.h>
#include "board.h"
#include "input.h"

#define WIDTH  80
#define HEIGHT 25
#define FRAMERATE 100

int main() {
    // コンソールの初期化
    initConsole(WIDTH, HEIGHT);

    // 入力状態の初期化
    InputState inputState;
    initInput(&inputState);

    // ゲーム状態の初期化
    Init();

    char frameBuffer[WIDTH * HEIGHT];
    int frame = 0;

    while (1) {
        // 現在のメニュー状態に応じた処理
        MenuState currentState = getMenuState();
        switch (currentState) {
            case MENU_MAIN:
            case MENU_MODE_SELECT:
            case MENU_CPU_LEVEL:
                handleMenuInput(&inputState, &field);
                break;
            case MENU_GAME:
                handleGameInput(&inputState, &field);
                break;
        }

        // フレーム描画実行
        if (frame > 8) frame = 0;
        drawBoard(frameBuffer, &field);
        if (currentState == MENU_GAME) {
            drawCursor(frameBuffer, inputState.cursorX, inputState.cursorY);
        }
        drawFrameBuffer(frameBuffer, frame++);

        Sleep(FRAMERATE); // FPS制御
    }

    // コンソールのクリーンアップ
    cleanupConsole();

    return 0;
}

