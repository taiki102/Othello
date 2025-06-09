#pragma once
#include "utility.h"

typedef enum {
    Menu,
    VS,
    CPU,
}Step_Mode; // モード

typedef enum {
    Info,
    Input,
    End
} Step_input; // 状態ループ

typedef enum {
    Type_A,
    Type_B,
    Type_C,
}CPU_TYPE; // CPUタイプ

// 石の状態
typedef enum {
    EMPTY = ' ',
    BLACK = '●',
    WHITE = '○'
} Stone;

// 盤面の状態
typedef struct {
    Stone board[8][8];
    int blackCount;
    int whiteCount;
    bool canPlace[8][8];
} BoardState;

typedef struct {
    Step_Mode step_mode; 
    Step_input step_input;
    CPU_TYPE cpu_type;

    int turn; // ターン数
    bool Is_A_Turn; // A の ターンか
    BoardState boardState;
} field_game;

field_game field;

// ゲームの初期化
void Init();

// 盤面の初期化
void initBoard(BoardState* state);

// 石を置ける場所のチェック
bool checkCanPlace(BoardState* state, int x, int y, bool isBlack);

// 石を置く
bool placeStone(BoardState* state, int x, int y, bool isBlack);

// パスのチェック
bool checkPass(BoardState* state, bool isBlack);

// ゲーム終了のチェック
bool checkGameEnd(BoardState* state);

// CPUの手を決定
void decideCPUMove(BoardState* state, bool isBlack, CPU_TYPE cpuType);