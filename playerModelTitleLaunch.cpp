//=====================================
//
//テンプレート処理[playerModelTitleLaunch.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModelTitleLaunch.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERTITLELAUNCH_SPEED			(8.0f)
#define PLAYERTITLELAUNCH_DIR_START		(-0.2f)
#define PLAYERTITLELAUNCH_DIR_END		(5.0f)
#define PLAYERTITLELAUNCH_ACCEL_END		(60.0f)

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
void EnterPlayerModelTitleLaunch(PLAYERMODEL *player)
{
	player->cntFrame = 0;
}

/**************************************
更新処理
***************************************/
void UpdatePlayerModelTitleLaunch(PLAYERMODEL *player)
{
	player->cntFrame++;

	float t = (float)player->cntFrame / (float)PLAYERTITLELAUNCH_ACCEL_END;
	float dir = EaseInExponential(t, PLAYERTITLELAUNCH_DIR_START, PLAYERTITLELAUNCH_DIR_END);
	player->pos.z += dir * PLAYERTITLELAUNCH_SPEED;
}

/**************************************
退場処理
***************************************/
void ExitPlayerModelTitleLaunch(PLAYERMODEL *player)
{

}

