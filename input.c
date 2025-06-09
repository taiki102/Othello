#include "input.h"
#include <conio.h>

void initInput(InputState* state) {
    state->cursorX = 0;
    state->cursorY = 0;
    state->isSelecting = false;
}
void moveCursor(InputState* state, int dx, int dy) {
    state->cursorX = (state->cursorX + dx + 8) % 8;
    state->cursorY = (state->cursorY + dy + 8) % 8;
}

bool confirmSelection(InputState* state) {
    return state->isSelecting;
}

void handleMenuInput(InputState* state, field_game* game) {
    if (_kbhit()) {
        int key = _getch();
        if (key == 0 || key == 224) {  // 特殊キー
            key = _getch();
            switch (key) {
                case 72:  // 上矢印
                    state->cursorY = (state->cursorY - 1 + 3) % 3;
                    break;
                case 80:  // 下矢印
                    state->cursorY = (state->cursorY + 1) % 3;
                    break;
            }
        }
        else if (key == 13) {  // Enter
            MenuState currentState = getMenuState();
            switch (currentState) {
                case MENU_MAIN:
                    switch (state->cursorY) {
                        case 0:  // VS モード
                            game->step_mode = VS;
                            setMenuState(MENU_GAME);
                            break;
                        case 1:  // CPU モード
                            game->step_mode = CPU;
                            setMenuState(MENU_CPU_LEVEL);
                            break;
                        case 2:  // 終了
                            cleanupConsole();
                            exit(0);
                            break;
                    }
                    break;
                case MENU_CPU_LEVEL:
                    switch (state->cursorY) {
                        case 0:  // Type A
                            game->cpu_type = Type_A;
                            setMenuState(MENU_GAME);
                            break;
                        case 1:  // Type B
                            game->cpu_type = Type_B;
                            setMenuState(MENU_GAME);
                            break;
                        case 2:  // Type C
                            game->cpu_type = Type_C;
                            setMenuState(MENU_GAME);
                            break;
                        case 3:  // 戻る
                            setMenuState(MENU_MAIN);
                            break;
                    }
                    break;
            }
        }
    }
}

void handleGameInput(InputState* state, field_game* game) {
    if (_kbhit()) {
        int key = _getch();
        if (key == 0 || key == 224) {  // 特殊キー
            key = _getch();
            switch (key) {
                case 72:  // 上矢印
                    moveCursor(state, 0, -1);
                    break;
                case 80:  // 下矢印
                    moveCursor(state, 0, 1);
                    break;
                case 75:  // 左矢印
                    moveCursor(state, -1, 0);
                    break;
                case 77:  // 右矢印
                    moveCursor(state, 1, 0);
                    break;
            }
        }
        else if (key == 13) {  // Enter
            // 石を置く
            if (placeStone(&game->boardState, state->cursorX, state->cursorY, game->Is_A_Turn)) {
                game->turn++;
                game->Is_A_Turn = !game->Is_A_Turn;

                // パスのチェック
                if (checkPass(&game->boardState, game->Is_A_Turn)) {
                    game->Is_A_Turn = !game->Is_A_Turn;
                }

                // CPUの手番の場合
                if (game->step_mode == CPU && !game->Is_A_Turn) {
                    decideCPUMove(&game->boardState, game->Is_A_Turn, game->cpu_type);
                    game->turn++;
                    game->Is_A_Turn = !game->Is_A_Turn;

                    // パスのチェック
                    if (checkPass(&game->boardState, game->Is_A_Turn)) {
                        game->Is_A_Turn = !game->Is_A_Turn;
                    }
                }

                // ゲーム終了のチェック
                if (checkGameEnd(&game->boardState)) {
                    // TODO: ゲーム終了処理
                    setMenuState(MENU_MAIN);
                }
            }
        }
        else if (key == 27) {  // ESC
            setMenuState(MENU_MAIN);
        }
    }
}
