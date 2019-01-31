//=====================================
//
//ネームエントリー入力処理[nameEntryInput.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "nameEntryController.h"
#include "rankingName.h"
#include "input.h"
#include "nameEntryCursor.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
入場処理
***************************************/
void OnEnterNameEntryInput(NAMEENTRY *entity)
{
	if (entity->highScore->playerName[entity->nameIndex] == CHARCHIP_NONE)
		entity->highScore->playerName[entity->nameIndex] = CHARCHIP_A;
}

/**************************************
更新処理
***************************************/
void OnUpdateNameEntryInput(NAMEENTRY *entity)
{
	//名前を選択
	int inputY = GetVerticalInputRepeat();
	int *chara = &entity->highScore->playerName[entity->nameIndex];
	*chara = WrapAround(CHARCHIP_A, CHARCHIP_QUESTION, *chara - inputY);

	//文字送りを検出
	int inputX = GetHorizontalInputRepeat();
	entity->nameIndex = Clamp(0, DATACONTAINER_PLAYERNAME_MAX, entity->nameIndex + inputX);

	//カーソルをセット
	SetNameEntryCursor(entity->scoreIndex, entity->nameIndex);

	//状態遷移
	if (inputX == 0)
		return;

	if (entity->nameIndex == DATACONTAINER_PLAYERNAME_MAX)
	{
		SetStateNameEntryCursor(false);
		ChangeStateNameEntryController(NAMEENTRY_END);
	}
	else
	{
		ChangeStateNameEntryController(NAMEENTRY_INPUT);
	}
}
