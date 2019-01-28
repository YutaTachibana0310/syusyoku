//=====================================
//
//バトルボーナスタイム開始待機処理[battleWaitBonusTimeBegin.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "battleController.h"
#include "bonusCube.h"

/**************************************
マクロ定義
***************************************/

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
void OnEnterBattleWaitBonusTimeBegin(BATTLECONTROLLER *controller)
{
	cntFrame = 0;
}

/**************************************
更新処理
***************************************/
void OnUpdateBattleWaitBonusTimeBegin(BATTLECONTROLLER *controller)
{
	cntFrame++;
	if (cntFrame % BATTLE_EMMITT_INTERBAL == 0)
		EmmittFromFuzzy(controller);

	//遷移処理
	if (IsAllBonusCubeDisable())
	{
		//全てのボーナスキューブが撃墜されたらボーナスタイムへ遷移
		ChangeStateBattleController(BattleBonusTime);
	}
	else if (CheckEscapedBonusCube())
	{
		//一つでもボーナスキューブを逃したらノーマルタイムへ遷移
		ChangeStateBattleController(BattleNormalTime);
	}
}
