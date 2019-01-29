//=====================================
//
//バトルコントローラ処理[battleController.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "battleController.h"
#include "fuzzyLogic.h"
#include "playerModel.h"
#include "debugWindow.h"
#include "battleCamera.h"
#include "enemyManager.h"
#include "bonusTelop.h"
#include "stageData.h"
#include "bgmManager.h"

#include "cubeObject.h"
#include "hardCubeObject.h"
#include "bonusCube.h"

/**************************************
マクロ定義
***************************************/
#define BATTLE_SPACE_LEFT_BORDER		(-300.0f)			//エネミーの生成座標範囲（左端）
#define BATTLE_SPACE_TOP_BORDER			(300.0f)			//エネミーの生成座標範囲（上端）
#define BATTLE_SPACE_RIGHT_BORDER		(300.0f)			//エネミーの生成座標範囲（右端）
#define BATTLE_SPACE_BOTTOM_BORDER		(-300.0f)			//エネミーの生成座標範囲（下端）
#define BATTLE_EMITTPOS_Z				(6000.0f)			//エネミー生成座標Z値

#define BATTLE_CUBEEMMITT_INTERBAL		(60)

#define BATTLE_CUBE_INITEMMITT_SPEED	(15.0f)

/**************************************
構造体定義
***************************************/
typedef void(*funcBattleController)(BATTLECONTROLLER *controller);

/**************************************
プロトタイプ宣言
***************************************/
//void EmmittOnBonusTime(void);
//void EmmittOnNormalTime(void);
//void EmittFromStageData(void);

//入場処理
void OnEnterBattleNormalTime(BATTLECONTROLLER *controller);
void OnEnterBattleWaitBonusTimeBegin(BATTLECONTROLLER *controller);
void OnEnterBattleBonusTime(BATTLECONTROLLER *controller);

//更新処理
void OnUpdateBattleNormalTime(BATTLECONTROLLER *controller);
void OnUpdateBattleWaitBonusTimeBegin(BATTLECONTROLLER *controller);
void OnUpdateBattleBonusTime(BATTLECONTROLLER *controller);

/**************************************
グローバル変数
***************************************/
//static bool isBonusTime;
//static bool countState;

static BATTLECONTROLLER controller;

//入場処理テーブル
static funcBattleController Enter[BattleStateMax] = {
	OnEnterBattleNormalTime,
	OnEnterBattleWaitBonusTimeBegin,
	OnEnterBattleBonusTime,
};

//更新処理テーブル
static funcBattleController Update[BattleStateMax] = {
	OnUpdateBattleNormalTime,
	OnUpdateBattleWaitBonusTimeBegin,
	OnUpdateBattleBonusTime,
};

/**************************************
初期化処理
***************************************/
void InitBattleController(int num)
{
	InitStageData(num);

	//生成範囲を分割してエネミーの生成座標を計算
	float spaceUnitWidth = SCREEN_WIDTH / BATTLE_SPACE_DIVIDE_NUM;
	float spaceUnitHeight = SCREEN_HEIGHT / BATTLE_SPACE_DIVIDE_NUM;

	for (int y = 0; y < BATTLE_SPACE_DIVIDE_NUM; y++)
	{
		for (int x = 0; x < BATTLE_SPACE_DIVIDE_NUM; x++)
		{
			controller.checkPos[y * BATTLE_SPACE_DIVIDE_NUM + x].x = spaceUnitWidth * (x + 0.5f);
			controller.checkPos[y * BATTLE_SPACE_DIVIDE_NUM + x].y = spaceUnitHeight * (y + 0.5f);

			controller.emmittPos[y * BATTLE_SPACE_DIVIDE_NUM + x].x = (BATTLE_SPACE_RIGHT_BORDER - BATTLE_SPACE_LEFT_BORDER) / BATTLE_SPACE_DIVIDE_NUM * (x + 0.5f) + BATTLE_SPACE_LEFT_BORDER;
			controller.emmittPos[y * BATTLE_SPACE_DIVIDE_NUM + x].y = (BATTLE_SPACE_TOP_BORDER - BATTLE_SPACE_BOTTOM_BORDER) / BATTLE_SPACE_DIVIDE_NUM * (y + 0.5f) + BATTLE_SPACE_BOTTOM_BORDER;
			controller.emmittPos[y * BATTLE_SPACE_DIVIDE_NUM + x].z = 5000.0f;
		}
	}

	//キューブを初期配置
	for (int z = 0; z < 5000; z += 100)
	{
		int x = rand() % 3;
		int y = rand() % 3;

		D3DXVECTOR3 pos = D3DXVECTOR3(controller.emmittPos[y * BATTLE_SPACE_DIVIDE_NUM + x].x, controller.emmittPos[y * BATTLE_SPACE_DIVIDE_NUM + x].y, z);
		EmmittCubeObject(1, &pos, BATTLE_CUBE_INITEMMITT_SPEED);
	}

	//各パラメータを初期化
	controller.cntFrame = 0;
	for (int i = 0; i < BATTLE_SPACE_MAX; i++)
	{
		controller.lastEmittFrame[i] = 0xffffffff;
	}

	controller.currentState = BattleNormalTime;
	controller.prevState = BattleNormalTime;
}

/**************************************
終了処理
***************************************/
void UninitBattleController(int num)
{
	UninitStageData(num);
}

/**************************************
更新処理
***************************************/
void UpdateBattleController(void)
{
	Update[controller.currentState](&controller);
}

/**************************************
状態遷移処理
***************************************/
void ChangeStateBattleController(int next)
{
	controller.currentState = next;
	Enter[controller.currentState](&controller);
}

/**************************************
ボーナスタイムかどうか
***************************************/
bool IsBonusTime(void)
{
	if (controller.currentState == BattleBonusTime)
		return true;
	else
		return false;
}

/**************************************
ファジィ理論でのキューブ放出処理
***************************************/
void EmmittFromFuzzy(BATTLECONTROLLER *controller)
{
	float valueLength[BATTLE_SPACE_MAX];
	float valueTime[BATTLE_SPACE_MAX];
	float fuzzyValue[BATTLE_SPACE_MAX];
	int decidedPos = 0;

	D3DXVECTOR3 playerPos;
	float maxValue = -9999.9f;

	D3DXVec3TransformCoord(&playerPos, &GetPlayerAdr(0)->pos, &GetBattleCameraView());
	D3DXVec3TransformCoord(&playerPos, &playerPos, &GetBattleCameraProjection());
	TranslateViewPort(&playerPos, &playerPos);

	//各生成座標に対してファジィ理論で重みを計算
	for (int i = 0; i < BATTLE_SPACE_MAX; i++)
	{
		float length = D3DXVec2Length(&(D3DXVECTOR2)(playerPos - controller->checkPos[i]));
		float elapsedTime = (float)(controller->cntFrame - controller->lastEmittFrame[i]);

		valueLength[i] = fFuzzyTriangle(length, BATTLE_FUZZY_NEAR_BORDER, BATTLE_FUZZY_MIDDLE_BORDER, BATTLE_FUZZY_FAR_BORDER);
		valueTime[i] = fFuzzyRightGrade(elapsedTime, BATTLE_FUZZY_RECENTLY_BORDER, BATTLE_FUZZY_LATELY_BORDER);
		fuzzyValue[i] = valueLength[i] * valueTime[i];
		if (maxValue < fuzzyValue[i])
		{
			decidedPos = i;
			maxValue = fuzzyValue[i];
		}
	}

	EmmittCubeObject(BATTLE_CUBEEMMITT_NUM, &(controller->emmittPos[decidedPos]), BATTLE_CUBEEMMITT_SPEED);
	controller->lastEmittFrame[decidedPos] = controller->cntFrame;
}