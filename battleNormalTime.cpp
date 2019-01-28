//=====================================
//
//バトル通常時更新処理[battleNormalTime.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "battleController.h"
#include "fuzzyLogic.h"
#include "playerModel.h"
#include "enemyManager.h"
#include "stageData.h"
#include "hardCubeObject.h"
#include "bonusCube.h"
#include "bonusTelop.h"

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
void EmittFromStageData(BATTLECONTROLLER *controller);
void EmmittFromFuzzy(BATTLECONTROLLER *controller);

/**************************************
入場処理
***************************************/
void OnEnterBattleNormalTime(BATTLECONTROLLER *controller)
{
	//ボーナスタイム終了テロップを流す
}

/**************************************
更新処理
***************************************/
void OnUpdateBattleNormalTime(BATTLECONTROLLER *controller)
{
	controller->cntFrame++;
	EmittFromStageData(controller);

	if (controller->cntFrame % BATTLE_EMMITT_INTERBAL != 0)
		EmmittFromFuzzy(controller);

}

/**************************************
ステージデータからのキューブ放出処理
***************************************/
void EmittFromStageData(BATTLECONTROLLER *controller)
{
	/*
	自動生成のプログラムが間に合わなさそうなので
	ステージデータを手打ちで作成して使用
	*/
	int cntData = 0;
	STAGE_DATA *data = NULL;
	cntData = UpdateStageData(&data, controller->cntFrame);
	for (int i = 0; i < cntData; i++, data++)
	{
		if (data->type < HardCubeTypeMax)
			SetHardCubeObjectFromData(data);
		else
			SetBonusCube(&data->initPos);
	}
}