//=====================================
//
//ハードキューブベジェ処理[hardCubeBezier.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "hardCubeObject.h"

/**************************************
マクロ定義
***************************************/
#define HARDCUBE_BEZIER_DURATION		(300)

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
void OnEnterHardCubeBezier(HARD_CUBE_OBJECT *ptr)
{
	ptr->cntFrame = 0;
	ptr->scale = 0.5f;
	ptr->startPos = ptr->pos;
}

/**************************************
更新処理
***************************************/
void OnUpdateHardCubeBezier(HARD_CUBE_OBJECT *ptr)
{
	ptr->cntFrame++;
	float t = (float)ptr->cntFrame / (float)HARDCUBE_BEZIER_DURATION;

	//移動先の座標を3次ベジェ曲線により求める
	ptr->pos =
		powf(1 - t, 3) * ptr->startPos
		+ 3 * powf(1 - t, 2) * t * ptr->controller1
		+ 3 * (1 - t) * t * t * ptr->controller2
		+ t * t * t * ptr->goalPos;

	if (ptr->cntFrame == HARDCUBE_BEZIER_DURATION)
	{
		DisableHardCube(ptr);
	}
}