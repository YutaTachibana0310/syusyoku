//=====================================
//
//ネームエントリーコントローラヘッダ[nameEntryController.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _NAMEENTYCONTROLLER_H_
#define _NAMEENTYCONTROLLER_H_

#include "main.h"
#include "dataContainer.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct {
	int scoreIndex;
	int nameIndex;
	DATA_HIGHSCRE *highScore = NULL;
	int currentState;
	int prevState;
}NAMEENTRY;

enum NAMEENTRY_STATE {
	NAMEENTRY_INPUT,
	NAMEENTRY_END,
	NAMEENTRY_STATEMAX
};
/**************************************
プロトタイプ宣言
***************************************/
void InitNameEntryController(int num);
void UninitNameEntryController(int num);
void UpdateNameEntryController(void);
void DrawNameEntryController(void);
void ChangeStateNameEntryController(int next);

//入場処理
void OnEnterNameEntryInput(NAMEENTRY *entity);
void OnEnterNameEntryEnd(NAMEENTRY *entity);

//更新処理
void OnUpdateNameEntryInput(NAMEENTRY *entity);
void OnUpdateNameEntryEnd(NAMEENTRY *entity);
#endif