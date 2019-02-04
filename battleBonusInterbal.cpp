//=====================================
//
//テンプレート処理[battleBonusInterbal.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "battleController.h"

/**************************************
マクロ定義
***************************************/
#define BATTLE_BONUSINTERBAL_DURATION	(120)

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
入場処理
***************************************/
void OnEnterBattleBonusInterbal(BATTLECONTROLLER *entity)
{
	cntFrame = 0;
}

/**************************************
更新処理
***************************************/
void OnUpdateBattleBonusInterbal(BATTLECONTROLLER *entity)
{
	cntFrame++;

	if (cntFrame == BATTLE_BONUSINTERBAL_DURATION)
	{
		ChangeStateBattleController(BattleBonusTime);
	}
}