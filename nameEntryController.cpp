//=====================================
//
//ネームエントリーコントローラ処理[nameEntryController.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "nameEntryController.h"
#include "dataContainer.h"
#include "rankingName.h"
#include "input.h"
#include "nameEntryCursor.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef void(*funcNameEntryContoller)(NAMEENTRY*);

/**************************************
グローバル変数
***************************************/
static NAMEENTRY entity;

//入場処理テーブル
static funcNameEntryContoller Enter[NAMEENTRY_STATEMAX] = {
	OnEnterNameEntryInput,
	OnEnterNameEntryEnd
};

//更新処理table
static funcNameEntryContoller Update[NAMEENTRY_STATEMAX] = {
	OnUpdateNameEntryInput,
	OnUpdateNameEntryEnd,
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitNameEntryController(int num)
{
	//ランキングの更新を確認
	bool flgUpdate = CheckUpdateRanking(&entity.scoreIndex);

	//更新があった場合
	if (flgUpdate)
	{
		entity.highScore = GetHighScore(entity.scoreIndex);

		//各パラメータをセット
		entity.nameIndex = 0;
		SetStateNameEntryCursor(true);

		//プレイヤー名を初期化
		for (int i = 0; i < DATACONTAINER_PLAYERNAME_MAX; i++)
		{
			entity.highScore->playerName[i] = 31;
		}

		//入力状態へ遷移
		ChangeStateNameEntryController(NAMEENTRY_INPUT);
	}

	//更新がなかった場合
	else
	{
		ChangeStateNameEntryController(NAMEENTRY_END);
	}
}

/**************************************
終了処理
***************************************/
void UninitNameEntryController(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdateNameEntryController(void)
{
	Update[entity.currentState](&entity);
}

/**************************************
描画処理
***************************************/
void DrawNameEntryController(void)
{

}

/**************************************
状態遷移処理
***************************************/
void ChangeStateNameEntryController(int next)
{
	entity.currentState = next;
	Enter[entity.currentState](&entity);
}