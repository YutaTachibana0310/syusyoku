//=====================================
//
//バトル視点変更ステート処理[battleChangeViewMode.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "battleController.h"
#include "cautionTelop.h"

/**************************************
マクロ定義
***************************************/
#define BATTLE_CHANGEVIEWMODE_DURATION		(270)
#define BATTLE_CHANGEVIEW_TIMING			(240)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static int cntFrame;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void OnEnterBattleChangeView(BATTLECONTROLLER *entity)
{
	cntFrame = 0;
	SetCautionTelop();
}

/**************************************
更新処理
***************************************/
void OnUpdateBattleChangeView(BATTLECONTROLLER *entity)
{
	cntFrame++;

	if (cntFrame == BATTLE_CHANGEVIEW_TIMING)
	{
		ChangeViewModeBattleController(entity->nextViewMode);
	}

	if (cntFrame == BATTLE_CHANGEVIEWMODE_DURATION)
	{
		ChangeStateBattleController(BattleNormalTime);
	}
}
