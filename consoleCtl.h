#ifndef CONSOLE_CTL_H
#define CONSOLE_CTL_H

#include <windows.h>

// 画面サイズの定数
#define CONSOLE_WIDTH  80
#define CONSOLE_HEIGHT 25
#define CONSOLE_FRAMERATE 100

// 描画関数の型定義
typedef void (*DrawFunction)(char* buffer, int frame);

// メニュー選択の型定義
typedef enum {
    MENU_MAIN,
    MENU_MODE_SELECT,
    MENU_CPU_LEVEL,
    MENU_GAME
} MenuState;

// コンソールの初期化
void initConsole(int width, int height);

// 描画関数の設定
void setDrawFunction(DrawFunction func);

// フレームバッファの描画
void drawFrameBuffer(char* buffer, int frame);

// コンソールのクリーンアップ
void cleanupConsole(void);

// メニュー状態の設定
void setMenuState(MenuState state);

// メニュー状態の取得
MenuState getMenuState(void);

#endif // CONSOLE_CTL_H 