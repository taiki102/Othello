#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

// 方向の定義
static const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
static const int dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

void Init() {
	field.turn = 0;
	field.Is_A_Turn = true;
	field.step_mode = Menu;
	field.step_input = Info;
	field.cpu_type = Type_A;
	initBoard(&field.boardState);
}

void initBoard(BoardState* state) {
	// 盤面の初期化
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			state->board[y][x] = EMPTY;
			state->canPlace[y][x] = false;
		}
	}

	// 初期配置
	state->board[3][3] = WHITE;
	state->board[3][4] = BLACK;
	state->board[4][3] = BLACK;
	state->board[4][4] = WHITE;

	// 石の数を初期化
	state->blackCount = 2;
	state->whiteCount = 2;

	// 置ける場所を更新
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			state->canPlace[y][x] = checkCanPlace(state, x, y, true);
		}
	}
}

bool checkCanPlace(BoardState* state, int x, int y, bool isBlack) {
	if (state->board[y][x] != EMPTY) return false;

	Stone myStone = isBlack ? BLACK : WHITE;
	Stone enemyStone = isBlack ? WHITE : BLACK;

	for (int dir = 0; dir < 8; dir++) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		bool foundEnemy = false;

		while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
			if (state->board[ny][nx] == enemyStone) {
				foundEnemy = true;
			}
			else if (state->board[ny][nx] == myStone && foundEnemy) {
				return true;
			}
			else {
				break;
			}
			nx += dx[dir];
			ny += dy[dir];
		}
	}
	return false;
}

bool placeStone(BoardState* state, int x, int y, bool isBlack) {
	if (!checkCanPlace(state, x, y, isBlack)) return false;

	Stone myStone = isBlack ? BLACK : WHITE;
	Stone enemyStone = isBlack ? WHITE : BLACK;
	state->board[y][x] = myStone;

	// 石を裏返す
	for (int dir = 0; dir < 8; dir++) {
		int nx = x + dx[dir];
		int ny = y + dy[dir];
		bool foundEnemy = false;
		bool canFlip = false;

		while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
			if (state->board[ny][nx] == enemyStone) {
				foundEnemy = true;
			}
			else if (state->board[ny][nx] == myStone && foundEnemy) {
				canFlip = true;
				break;
			}
			else {
				break;
			}
			nx += dx[dir];
			ny += dy[dir];
		}

		if (canFlip) {
			nx = x + dx[dir];
			ny = y + dy[dir];
			while (state->board[ny][nx] == enemyStone) {
				state->board[ny][nx] = myStone;
				nx += dx[dir];
				ny += dy[dir];
			}
		}
	}

	// 石の数を更新
	state->blackCount = 0;
	state->whiteCount = 0;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (state->board[y][x] == BLACK) state->blackCount++;
			if (state->board[y][x] == WHITE) state->whiteCount++;
		}
	}

	// 置ける場所を更新
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			state->canPlace[y][x] = checkCanPlace(state, x, y, !isBlack);
		}
	}

	return true;
}

bool checkPass(BoardState* state, bool isBlack) {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (checkCanPlace(state, x, y, isBlack)) {
				return false;
			}
		}
	}
	return true;
}

bool checkGameEnd(BoardState* state) {
	// 盤面が埋まっている
	if (state->blackCount + state->whiteCount == 64) return true;

	// 両者とも置けない
	bool blackCanPlace = false;
	bool whiteCanPlace = false;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (checkCanPlace(state, x, y, true)) blackCanPlace = true;
			if (checkCanPlace(state, x, y, false)) whiteCanPlace = true;
		}
	}
	return !blackCanPlace && !whiteCanPlace;
}

// CPUの評価関数
int evaluatePosition(BoardState* state, bool isBlack) {
	int score = 0;
	Stone myStone = isBlack ? BLACK : WHITE;

	// 角の評価
	const int corners[4][2] = {{0,0}, {0,7}, {7,0}, {7,7}};
	for (int i = 0; i < 4; i++) {
		if (state->board[corners[i][1]][corners[i][0]] == myStone) {
			score += 100;
		}
	}

	// 端の評価
	for (int x = 0; x < 8; x++) {
		if (state->board[0][x] == myStone) score += 10;
		if (state->board[7][x] == myStone) score += 10;
	}
	for (int y = 0; y < 8; y++) {
		if (state->board[y][0] == myStone) score += 10;
		if (state->board[y][7] == myStone) score += 10;
	}

	// 石の数の評価
	score += isBlack ? state->blackCount : state->whiteCount;

	return score;
}

void decideCPUMove(BoardState* state, bool isBlack, CPU_TYPE cpuType) {
	int bestScore = -9999;
	int bestX = -1;
	int bestY = -1;

	// ランダムな手を生成
	srand(time(NULL));
	int randomMoves[64][2];
	int moveCount = 0;

	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			if (checkCanPlace(state, x, y, isBlack)) {
				randomMoves[moveCount][0] = x;
				randomMoves[moveCount][1] = y;
				moveCount++;
			}
		}
	}

	if (moveCount == 0) return;

	// CPUのレベルに応じた手の選択
	switch (cpuType) {
		case Type_A:  // 初級：完全ランダム
			{
				int move = rand() % moveCount;
				bestX = randomMoves[move][0];
				bestY = randomMoves[move][1];
			}
			break;

		case Type_B:  // 中級：評価関数を使用
			for (int i = 0; i < moveCount; i++) {
				BoardState tempState = *state;
				if (placeStone(&tempState, randomMoves[i][0], randomMoves[i][1], isBlack)) {
					int score = evaluatePosition(&tempState, isBlack);
					if (score > bestScore) {
						bestScore = score;
						bestX = randomMoves[i][0];
						bestY = randomMoves[i][1];
					}
				}
			}
			break;

		case Type_C:  // 上級：より深い読み
			for (int i = 0; i < moveCount; i++) {
				BoardState tempState = *state;
				if (placeStone(&tempState, randomMoves[i][0], randomMoves[i][1], isBlack)) {
					int score = evaluatePosition(&tempState, isBlack);
					// 相手の最善手を考慮
					for (int y = 0; y < 8; y++) {
						for (int x = 0; x < 8; x++) {
							if (checkCanPlace(&tempState, x, y, !isBlack)) {
								BoardState tempState2 = tempState;
								if (placeStone(&tempState2, x, y, !isBlack)) {
									score -= evaluatePosition(&tempState2, !isBlack);
								}
							}
						}
					}
					if (score > bestScore) {
						bestScore = score;
						bestX = randomMoves[i][0];
						bestY = randomMoves[i][1];
					}
				}
			}
			break;
	}

	if (bestX != -1 && bestY != -1) {
		placeStone(state, bestX, bestY, isBlack);
	}
}