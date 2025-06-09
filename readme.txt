
このコンソールアプリはオセロを複数のモードで遊ぶことができる
・CPU戦(ランダムに抽選される　アルゴリズムと名前)　
    { MinMax , ... }
・対人戦

特徴
・モード選択
・コンソールUI
・便利な場所選択ができる(1-9 +)

Othello.v2 構造
src/
	main.c
    ｜  ∟consoleCtl.c
    ｜
    game.c(utility.h)
        ∟board.c
	    ∟input.c

header/
    frameCtl.h
	board.h
	input.h
	utility.h (Vec2Int)
	
			 -Othello-
 -----------------------------------
     |　|　|　|　|　|　|　|　| 
     |　|　|　|　|　|　|　|　| 
     |　|　|　|　|　|　|　|　| 
     |　|　|　|○|●|　|　|　| 
     |　|　|　|●|○|　|　|　| PlayerAの番
     |　|　|　|　|　|　|　|　| 
     |　|　|　|　|　|　|　|　| 
     |　|　|　|　|　|　|　|　| 
-----------------------------------
     操作　