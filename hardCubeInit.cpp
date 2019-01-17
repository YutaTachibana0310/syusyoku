//=====================================
//
//ハードキューブ初期状態処理[hardCubeInit.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "hardCubeObject.h"

/**************************************
マクロ定義
***************************************/
#define HARDCUBE_SCALE_MAX			(2.0f)
#define HARDCUBE_SCALE_MIN			(0.5f)
#define HARDCUBE_INIT_HP			(10.0f)						//初期HP

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
void OnEnterHardCubeInit(HARD_CUBE_OBJECT *ptr)
{
	ptr->scale = RandomRangef(HARDCUBE_SCALE_MIN, HARDCUBE_SCALE_MAX);
	ptr->hp = HARDCUBE_INIT_HP * ptr->scale;
	ChangeStateHardCube(ptr, HardCubeMove);
}

/**************************************
更新処理
***************************************/
void OnUpdateHardCubeInit(HARD_CUBE_OBJECT *ptr)
{

}