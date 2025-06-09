#include "consoleCtl.h"
#include "game.h"
#include <stdio.h>
#include <string.h>

#define WIDTH  80
#define HEIGHT 25
#define FRAMERATE 100

// 描画関数の型定義
typedef void (*DrawFunction)(char* buffer, int frame);

// 現在の描画関数を保持する変数
static DrawFunction currentDrawFunction = NULL;

// コンソールバッファ
static HANDLE buffers[2];
static CHAR_INFO screen[WIDTH * HEIGHT];
static int currentBuffer = 0;

// メニュー状態
static MenuState currentMenuState = MENU_MAIN;

// メニュー項目の定義
static const char* mainMenuItems[] = {
    "オセロ",
    "1. VS モード",
    "2. CPU モード",
    "3. 終了",
    NULL
};

static const char* cpuLevelItems[] = {
    "CPU レベル選択",
    "1. 初級 (Type A)",
    "2. 中級 (Type B)",
    "3. 上級 (Type C)",
    "4. 戻る",
    NULL
};

const char* frames[10] = {
    "     \n  *  \n     \n     \n     \n",
    "     \n *#* \n  *  \n *#* \n     \n",
    "  *  \n*#O#*\n #X# \n*#O#*\n  *  \n",
    " *#* \n#XOX#\nX#X#X\n#XOX#\n *#* \n",
    "#XOX#\nX#X#X\nOXOXO\nX#X#X\n#XOX#\n",
    "XXXXX\nXOXOX\nOXOXO\nXOXOX\nXXXXX\n",
    "X   X\n O O \nX O X\n O O \nX   X\n",
    "     \n  X  \n X X \n  X  \n     \n",
    "     \n     \n  .  \n     \n     \n",
    "     \n     \n     \n     \n     \n"
};

const char* catBlinkFrames[12] = {
    // 猫の静止画像 0フレード目
    " /\\_/\\  \n( o.o ) \n > ^ <  \n         \n         \n",

    // 猫の静止画像 1フレード目
    " /\\_/\\  \n( o.o ) \n > ^ <  \n         \n         \n",

    // 猫の静止画像 2フレード目
    " /\\_/\\  \n( -.- ) \n > ^ <  \n         \n         \n",

    // 猫の静止画像 3フレード目
    " /\\_/\\  \n( -.- ) \n > ^ <  \n         \n         \n",

    // 猫の静止画像 4フレード目
    " /\\_/\\  \n( _ _ )\n > ^ <  \n         \n         \n",

    // 猫の静止画像 5フレード目
    " /\\_/\\  \n( _ _ )\n > ^ <  \n         \n         \n",

    // 猫の静止画像 4フレード目
    " /\\_/\\  \n( _ _ )\n > ^ <  \n         \n         \n",

    // 猫の静止画像 5フレード目
    " /\\_/\\  \n( _ _ )\n > ^ <  \n         \n         \n",

    // 猫の静止画像 6フレード目
    " /\\_/\\  \n( - - ) \n > ^ <  \n         \n         \n",

    // 猫の静止画像 7フレード目
    " /\\_/\\  \n( -.- ) \n > ^ <  \n         \n         \n",

    // 猫の静止画像 8フレード目
    " /\\_/\\  \n( o.o ) \n > ^ <  \n         \n         \n",

    // 猫の静止画像 9フレード目
    " /\\_/\\  \n( o.o ) \n > ^ <  \n         \n         \n"
};

const char* moonFrames[9] = {
    "  O          \n",
    "  OT         \n",
    "  OTHE       \n",
    "  OTHEL      \n",
    "  OTHELLO    \n",
    "= OTHELLO =  \n",
    "             \n",
    "= OTHELLO =  \n",
    "             \n"
};

// メインメニューの描画
static void drawMainMenu(char* buffer, int frame) {
    memset(buffer, ' ', WIDTH * HEIGHT);

    // タイトル
    const char* title = "OTHELLO";
    int titleX = (WIDTH - strlen(title)) / 2;
    memcpy(buffer + 5 * WIDTH + titleX, title, strlen(title));

    // メニュー項目
    int y = 8;
    for (const char** item = mainMenuItems; *item != NULL; item++) {
        int x = (WIDTH - strlen(*item)) / 2;
        memcpy(buffer + y * WIDTH + x, *item, strlen(*item));
        y++;
    }

    // アニメーション効果
    if (frame % 2 == 0) {
        const char* cursor = ">";
        int cursorX = (WIDTH - strlen(mainMenuItems[1])) / 2 - 2;
        memcpy(buffer + 8 * WIDTH + cursorX, cursor, strlen(cursor));
    }
}

// CPUレベル選択メニューの描画
static void drawCpuLevelMenu(char* buffer, int frame) {
    memset(buffer, ' ', WIDTH * HEIGHT);

    // タイトル
    const char* title = "CPU LEVEL SELECT";
    int titleX = (WIDTH - strlen(title)) / 2;
    memcpy(buffer + 5 * WIDTH + titleX, title, strlen(title));

    // メニュー項目
    int y = 8;
    for (const char** item = cpuLevelItems; *item != NULL; item++) {
        int x = (WIDTH - strlen(*item)) / 2;
        memcpy(buffer + y * WIDTH + x, *item, strlen(*item));
        y++;
    }

    // アニメーション効果
    if (frame % 2 == 0) {
        const char* cursor = ">";
        int cursorX = (WIDTH - strlen(cpuLevelItems[1])) / 2 - 2;
        memcpy(buffer + 8 * WIDTH + cursorX, cursor, strlen(cursor));
    }
}

// ゲーム画面の描画
static void drawGameScreen(char* buffer, int frame) {
    memset(buffer, ' ', WIDTH * HEIGHT);

    // ゲーム情報
    char info[32];
    snprintf(info, sizeof(info), "Turn: %d", field.turn);
    memcpy(buffer + 1 * WIDTH + 2, info, strlen(info));

    // プレイヤー情報
    const char* player = field.Is_A_Turn ? "Player A" : "Player B";
    memcpy(buffer + 2 * WIDTH + 2, player, strlen(player));

    // ゲームモード
    const char* mode = field.step_mode == VS ? "VS Mode" : "CPU Mode";
    memcpy(buffer + 3 * WIDTH + 2, mode, strlen(mode));

    // オセロ盤の描画
    // TODO: 実際のオセロ盤の描画を実装
}

// メニュー状態に応じた描画関数の選択
static void selectDrawFunction(void) {
    switch (currentMenuState) {
        case MENU_MAIN:
            setDrawFunction(drawMainMenu);
            break;
        case MENU_MODE_SELECT:
            setDrawFunction(drawMainMenu);  // 一時的に同じ描画関数を使用
            break;
        case MENU_CPU_LEVEL:
            setDrawFunction(drawCpuLevelMenu);
            break;
        case MENU_GAME:
            setDrawFunction(drawGameScreen);
            break;
    }
}

void setMenuState(MenuState state) {
    currentMenuState = state;
    selectDrawFunction();
}

MenuState getMenuState(void) {
    return currentMenuState;
}

void setConsoleSize(int width, int height) {
    HANDLE hwnd = GetConsoleWindow();
    Sleep(10);//If you execute these code immediately after the program starts, you must wait here for a short period of time, otherwise GetWindow will fail. I speculate that it may be because the console has not been fully initialized.
    HWND owner = GetWindow(hwnd, GW_OWNER);
    if (owner == NULL) {
        // Windows 10
        SetWindowPos(hwnd, 0x00, 0, 0, width, height, SWP_NOZORDER | SWP_NOMOVE);
    }
    else {
        // Windows 11
        SetWindowPos(owner, 0x00, 0, 0, 800, 500, SWP_NOZORDER | SWP_NOMOVE);
    }
}

// 描画関数を設定する関数
void setDrawFunction(DrawFunction func) {
    currentDrawFunction = func;
}

// デフォルトの描画関数
void defaultDrawFrame(char* buffer, int frame) {
    memset(buffer, ' ', WIDTH * HEIGHT);  // 全画面クリア

    // フレーム番号を1行目に表示（右寄せ）
    char text[32];
    snprintf(text, sizeof(text), "Frame: %d", frame);
    memcpy(buffer + 1 * WIDTH + (WIDTH - strlen(text)) / 2, text, strlen(text));

    // アニメーション表示（5行目くらいから下）
    const char* anim = moonFrames[frame];
    int startRow = 5;  // 任意の表示開始行
    int col = (WIDTH - 5) / 2;  // 中央寄せ（5文字分オフセット）

    int row = 0;
    while (*anim && startRow + row < HEIGHT) {
        if (*anim == '\n') {
            row++;
            anim++;
            continue;
        }

        if (col >= 0 && row < 8) {
            buffer[(startRow + row) * WIDTH + col] = *anim;
        }
        anim++;
        col++;

        if (*anim == '\n') col = (WIDTH - 5) / 2;  // 次の行の開始
    }
}

void initConsole(int width, int height) {
    setConsoleSize(width, height);

    COORD bufferSize = { WIDTH, HEIGHT };
    COORD bufferCoord = { 0, 0 };

    for (int i = 0; i < 2; ++i) {
        buffers[i] = CreateConsoleScreenBuffer(
            GENERIC_READ | GENERIC_WRITE, 0, NULL,
            CONSOLE_TEXTMODE_BUFFER, NULL);
        SetConsoleScreenBufferSize(buffers[i], bufferSize);
    }

    // デフォルトの描画関数を設定
    setDrawFunction(drawMainMenu);
}

void drawFrameBuffer(char* buffer, int frame) {
    if (currentDrawFunction != NULL) {
        currentDrawFunction(buffer, frame);
    }

    // CHAR_INFO に変換
    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        screen[i].Char.AsciiChar = buffer[i];
        screen[i].Attributes = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    }

    // バッファに描画
    COORD bufferSize = { WIDTH, HEIGHT };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, WIDTH - 1, HEIGHT - 1 };

    WriteConsoleOutputA(buffers[currentBuffer],
        screen, bufferSize, bufferCoord, &writeRegion);

    // 画面表示切り替え
    SetConsoleActiveScreenBuffer(buffers[currentBuffer]);

    // 次のバッファへ
    currentBuffer = 1 - currentBuffer;
}

void cleanupConsole(void) {
    for (int i = 0; i < 2; ++i) {
        CloseHandle(buffers[i]);
    }
}
