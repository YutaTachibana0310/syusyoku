//=====================================
//
//ハードキューブ突進処理[hardCubeCharge.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "hardCubeObject.h"
#include "playerModel.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define HARDCUBE_CHARGE_DURATION		(60)
#define HARDCUBE_CHARGE_SPEED			(10.0f)

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
void OnEnterHardCubeCharge(HARD_CUBE_OBJECT *ptr)
{
	ptr->cntFrame = 0;
	ptr->moveSpeed = 0;
	ptr->moveDir = GetPlayerAdr(0)->pos - ptr->pos;
	D3DXVec3Normalize(&ptr->moveDir, &ptr->moveDir);
}

/**************************************
更新処理
***************************************/
void OnUpdateHardCubeCharge(HARD_CUBE_OBJECT *ptr)
{
	ptr->cntFrame++;
	float t = (float)ptr->cntFrame / (float)HARDCUBE_CHARGE_DURATION;
	ptr->moveSpeed = EaseInCubic(t, 0.0f, HARDCUBE_CHARGE_SPEED);
	ptr->pos += ptr->moveDir * ptr->moveSpeed;
}