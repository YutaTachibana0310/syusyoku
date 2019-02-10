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
#include "dataContainer.h"

/**************************************
マクロ定義
***************************************/
#define BATTLE_DATATYPE_VIEWCHANGE		(999)
/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static int EmmittInterbal[DATACONTAINER_LOCKLEVEL_MAX] = {
	60, 30, 40, 30, 25, 20, 15
};

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
	controller->cntFrame[controller->viewMode]++;
	EmittFromStageData(controller);

	if (controller->cntFrame[controller->viewMode] % EmmittInterbal[GetLockonLevel()] == 0)
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
	cntData = UpdateStageData(&data, controller->cntFrame[controller->viewMode], controller->viewMode);
	for (int i = 0; i < cntData; i++, data++)
	{
		if (data->type < HardCubeTypeMax)
		{
			SetHardCubeObjectFromData(data);
		}
		else if (data->type == BATTLE_DATATYPE_VIEWCHANGE)
		{
			controller->nextViewMode = data->initPos.x;		//ステージデータのX座標に次の視点情報が格納されている
			ChangeStateBattleController(BattleChangeView);
		}
		else
		{
			ChangeStateBattleController(BattleWaitBonusTimeBegin);
			SetBonusCube();
		}
	}
}