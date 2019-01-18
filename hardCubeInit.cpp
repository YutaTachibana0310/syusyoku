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
#define HARDCUBE_SCALE_MIN			(0.2f)
#define HARDCUBE_INIT_HP			(10.0f)						//初期HP

#define HARDCUBE_SCALE_HOMING		(1.5f)
#define HARDCUBE_SCALE_CHARGE		(0.7f)

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
	/* スケールの自動生成は一旦マスクし、ステージデータに合わせてスケール設定 */
	//ptr->scale = RandomRangef(HARDCUBE_SCALE_MIN, HARDCUBE_SCALE_MAX);
	switch (ptr->type)
	{
	case HardCubeNormalType:
		ptr->scale = RandomRangef(HARDCUBE_SCALE_CHARGE, HARDCUBE_SCALE_HOMING);
		break;

	case HardCubeHomingType:
		ptr->scale = RandomRangef(HARDCUBE_SCALE_HOMING, HARDCUBE_SCALE_MAX);
		break;

	default:
		ptr->scale = RandomRangef(HARDCUBE_SCALE_MIN, HARDCUBE_SCALE_CHARGE);
	}

	ptr->hp = HARDCUBE_INIT_HP * ptr->scale;
	ChangeStateHardCube(ptr, HardCubeMove);
}

/**************************************
更新処理
***************************************/
void OnUpdateHardCubeInit(HARD_CUBE_OBJECT *ptr)
{

}