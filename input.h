#ifndef INPUT_H
#define INPUT_H

#include "game.h"
#include "consoleCtl.h"

// 入力状態の定義
typedef struct {
    int cursorX;
    int cursorY;
    bool isSelecting;
} InputState;

// 入力状態の初期化
void initInput(InputState* state);

// メニュー入力の処理
void handleMenuInput(InputState* state, field_game* game);

// ゲーム入力の処理
void handleGameInput(InputState* state, field_game* game);

// カーソルの移動
void moveCursor(InputState* state, int dx, int dy);

// 選択の確定
bool confirmSelection(InputState* state);

#endif // INPUT_H
