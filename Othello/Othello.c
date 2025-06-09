#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "utility.h"
#define MAXROW 8
#define DIR 8

#define bool int
#define true 1
#define false 0
#pragma region Vector2 Int

typedef struct {
    int x;
    int y;
} Vec2Int;

Vec2Int New(int x, int y) {
    Vec2Int v;
    v.x = x;
    v.y = y;
    return v;
}

Vec2Int Plus(Vec2Int x, Vec2Int y) {
    x.x += y.x;
    x.y += y.y;
    return x;
}

#pragma endregion
#pragma region Node.Vector2Int

typedef struct Node {
    Vec2Int data;
    struct Node* next;
} Node;

Node* createNode(Vec2Int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("メモリの確保に失敗しました。\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;
    return newNode;
}

void Add(Node** head, Vec2Int data) {
    Node* newNode = createNode(data);
    newNode->next = *head;
    *head = newNode;
}

void freeNode(Node* head) {
    Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp); // メモリ解放
    }
}
#pragma endregion

const Vec2Int dir[] = {
    {0, 1},    // 上
    {1, 1},    // 右上（斜め）
    {1, 0},    // 右
    {1, -1},   // 右下（斜め）
    {0, -1},   // 下
    {-1, -1},  // 左下（斜め）
    {-1, 0},   // 左
    {-1, 1}    // 左上（斜め）
};

typedef enum {
    Menu,
    VS,
    CPU,
}Step_Mode;
Step_Mode step_mode; // モード

typedef enum {
    DisplayInfo,
    Get,
    End
} Step_input;
Step_input step_input; // 状態ループ

int field[MAXROW][MAXROW]; // 盤面
int turn; // ターン数
bool Is_A_Turn; // A の ターンであるか

Vec2Int* list; // 配置候補リスト
Vec2Int target; // 過去配置
int length; // 候補の長さ
int Index; // 候補を選択する引数
int flg_menu; // メニューフラグ

int skipCount;
bool result;
bool flg_Update;

int pcount;
int ecount;

void Field_Setup() {
    for (int row = 0; row < MAXROW; row++) {
        for (int col = 0; col < MAXROW; col++) {
            field[row][col] = 0;
            if ((row == 3 && col == 3) || (row == 4 && col == 4))
                field[row][col] = 1;
            if ((row == 3 && col == 4) || (row == 4 && col == 3))
                field[row][col] = 2;
        }
    }
    turn = 1;
    Is_A_Turn = true;
    flg_menu = 1;
}

const char Icon[4][4] = { {" "}, {"○"},{"●"}, {"+"} };

void Board_Display(char* string1, char* string2) {
    for (int row = 0; row < MAXROW; row++) {
        printf("　　　　");
        printf("|");
        for (int col = 0; col < MAXROW; col++) {
            printf("%s|", Icon[field[row][col]]);
        }
        if (row == 3) {
            printf("    ");
            printf(string1);
        }
        if (row == 4) {
            printf("    ");
            printf(string2);
        }
        printf("\n");
    }
}

void Board_Select(bool IsPlus) {

    if (result) {
        step_mode = Menu;
        Field_Setup();
        return;
    }

    if (length == 0) {
        turn++;
        Is_A_Turn = Is_A_Turn ? 0 : 1;
        flg_Update = 1;
        return;
    }

    Index += IsPlus ? 1 : -1;
    if (Index < 0) {
        Index = length - 1;
    }
    if (Index >= length) {
        Index = 0;
    }
    Vec2Int addPoint = list[Index];
    field[target.x][target.y] = 0;
    field[addPoint.x][addPoint.y] = 3;
    target = addPoint;//list[Index];  
}

void Board_Decide() {
    int playerNum = Is_A_Turn ? 1 : 2;
    int enemyNum = Is_A_Turn ? 2 : 1;

    for (int i = 0; i < DIR; i++) {
        Node* tmplist = NULL;
        Vec2Int tg = Plus(target, dir[i]); // dir plus
        int flg = 0;

        if (field[tg.x][tg.y] == enemyNum) {

            while (!OutRange(tg)) {
                Add(&tmplist, tg);

                tg = Plus(tg, dir[i]);
                int check = field[tg.x][tg.y];
                if (check == playerNum) {
                    Add(&tmplist, tg);

                    Node* trg = tmplist;
                    while (trg != NULL) {
                        field[trg->data.x][trg->data.y] = playerNum;
                        trg = trg->next;
                    }
                    freeNode(tmplist);
                    break;
                }
                else if (check == 0)
                {
                    freeNode(tmplist);
                    break;
                }
            }
        }
    }

    field[target.x][target.y] = playerNum;
    turn++;
    Is_A_Turn = Is_A_Turn ? 0 : 1;
    flg_Update = 1;
}

void Screen_Display(char* string1,char* string2) {
    printf("            -OTHELLO-\n");
    printf("-----------------------------------\n");
    Board_Display(string1,string2);
    printf("-----------------------------------\n");
}

void NodeToArray(Node* head) {
    Node* temp = head;
    int index = 0;
    while (temp != NULL) {
        index++;
        temp = temp->next;
    }

    length = index;
    index = 0;

    free(list);
    list = (Vec2Int*)malloc(sizeof(Vec2Int) * length);

    temp = head;
    while (temp != NULL) {
        list[index] = temp->data;
        index++;
        temp = temp->next;
    }

    if (length != 0) {
        skipCount = 0;
        Index = 0;
        Vec2Int addPoint = list[Index];
        field[addPoint.x][addPoint.y] = 3;
        target = list[Index];
    }
    else
    {
        skipCount++;

        if (skipCount > 1) {
            printf("EndGame!!");
            pcount = 0;
            ecount = 0;
            for (int row = 0; row < MAXROW; row++) {
                for (int col = 0; col < MAXROW; col++) {
                    if (field[row][col] == 1) {
                        pcount++;
                    }
                    else if(field[row][col] == 2){
                        ecount++;
                    }
                }
            }
            Is_A_Turn = pcount > ecount ? 1 : 0;
            result = 1;
        }
        else
        {
            printf("Skipped!!");
        }
    }
}

bool OutRange(Vec2Int x) {
    return x.x < 0 || MAXROW <= x.x || x.y < 0 || MAXROW <= x.y;
}

void CalcSetArea() {
    if (!flg_Update) return;

    Node* head = NULL;

    int playerNum = Is_A_Turn ? 1 : 2;
    int enemyNum = Is_A_Turn ? 2 : 1;
    for (int row = 0; row < MAXROW; row++) {
        for (int col = 0; col < MAXROW; col++) {

            //自陣のマス
            if (field[row][col] == playerNum) {
                Vec2Int myCell = New(row, col);

                //一マスチェック
                for (int i = 0; i < DIR; i++) {
                    //Vec2Int tg = Plus(myCell, dir[i]); // dir plus

                    Vec2Int first = Plus(myCell, dir[i]);
                    if (!OutRange(first) && field[first.x][first.y] == enemyNum) {
                        Vec2Int tg = first;
                        while (1) {
                            tg = Plus(tg, dir[i]);
                            if (OutRange(tg)) break;

                            int check = field[tg.x][tg.y];
                            if (check == 0) {
                                Add(&head, tg);
                                break;
                            }
                            else if (check == playerNum) {
                                break;
                            }
                            // else 敵石なら continue
                        }
                    }
                }
            }
        }
    }

    NodeToArray(head);
    freeNode(head);
    flg_Update = 0;
}

void CPU_CalcCheck() {
    if (Is_A_Turn) return;

    srand(time(NULL)); // 乱数の初期化
    int r = rand() % (length - 1); // 0〜99の乱数

    target = list[r];
    Board_Decide();

    flg_Update = 0;
}

void Mode_Select(bool IsVS) {
    flg_menu = IsVS;
}

void Mode_Decide() {
    step_mode = flg_menu ? CPU : VS;
    flg_Update = 1;
}

void ScreenManager() {
    switch (step_input) {
    case DisplayInfo:
        switch (step_mode) {
        case Menu:
            Screen_Display("　　　  Home ",(flg_menu ? "●CPUゲーム←　○対人ゲーム" : "○CPUゲーム 　●対人ゲーム←"));
            break;
        case VS:{
            char text[40];

            if (!result) 
               snprintf(text, sizeof(text), "%d ターン目 - %s のターン", turn, Is_A_Turn ? "○-PlayerA" : "●-PlayerB ");    
            else
               snprintf(text, sizeof(text), "○ - :%d　● - :%d 勝者:%s", pcount, ecount, Is_A_Turn ? "○-PlayerA" : "●-PlayerB ");
            
            CalcSetArea();
            Screen_Display("　　　 対人ゲーム ", text);
        }
            break;
        case CPU:{
            char text[40];
            snprintf(text, sizeof(text), "%d ターン目 - %s のターン", turn, Is_A_Turn ? "Player" : "CPU");
            CalcSetArea();
            CPU_CalcCheck();
            Screen_Display("　　　 CPUゲーム ", text);
           // Sleep(1000);
        }
            break;
        default:
            break;
        }
        printf("操作 | ← → 選択 , Enter 決定\n");// : "操作 | A or D 選択 | F 決定 |\n
        step_input++;
        break;
    case Get:{
        int ch = getch();
        switch (step_mode) {
        case Menu:{
            if (ch == 0x4B) {
                printf("←\n");
                Mode_Select(true);
                step_input++;
            }
            else if (ch == 0x4D) {
                printf("→\n");
                Mode_Select(false);
                step_input++;
            }
            else if (ch == 0x0d) {
                printf("Enter\n");
                Mode_Decide();
                step_input++;
            }
        }
        break;
        case VS:{
            if (ch == 0x4B) {
                printf("←\n");
                Board_Select(true);
                step_input++;
            }
            else if (ch == 0x4D) {
                printf("→\n");
                Board_Select(false);
                step_input++;
            }
            else if (ch == 0x0d) {
                printf("Enter\n");
                Board_Decide();
                step_input++;
            }
            /*
            if (ch == 0x61) { // A
                printf("←\n");
                Index--;
                Move();
                step_input++;
            }
            else if (ch == 0x64) { // D
                printf("→\n");
                Index++;
                Move();
                step_input++;
            }
            else if (ch == 0x66) { // F
                step_input++;
            }
            */
        }
        break;
        case CPU:{
            if (ch == 0x4B) {
                printf("←\n");
                Board_Select(false);
                step_input++;
            }
            else if (ch == 0x4D) {
                printf("→\n");
                Board_Select(true);
                step_input++;
            }
            else if (ch == 0x0d) {
                printf("Enter\n");
                Board_Decide();
                step_input++;
            }
        }
        break;
        default:
            break;
        }
    }
    break;
    case End:
        system("cls");
        step_input = DisplayInfo;
    }
}

int main(void) {

    Field_Setup();

    while (1) {
        ScreenManager();
    }

    free(list);
    return 0;
}
