//=====================================
//
//ハードキューブ逃走状態処理[hardCubeEscape.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "hardCubeObject.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define HARDCUBE_ESCAPE_DURATION		(180)
#define HARDCUBE_ESCAPE_SPEED_MAX		(20.0f)
#define HARDCUBE_ESCAPE_Z_END			(10000.0f)

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
void OnEnterHardCubeEscape(HARD_CUBE_OBJECT *ptr)
{
	ptr->cntFrame = 0;
	ptr->moveSpeed = 0;
}

/**************************************
更新処理
***************************************/
void OnUpdateHardCubeEscape(HARD_CUBE_OBJECT *ptr)
{
	ptr->cntFrame++;
	float t = (float)ptr->cntFrame / HARDCUBE_ESCAPE_DURATION;
	ptr->moveSpeed = EaseInCubic(t, 0.0f, HARDCUBE_ESCAPE_SPEED_MAX);

	ptr->pos.z += ptr->moveSpeed;

	if (ptr->pos.z > HARDCUBE_ESCAPE_Z_END)
		DisableHardCube(ptr);
}