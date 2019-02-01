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

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static const float InitHp[HardCubeTypeMax] = {
	50.0f, 100.0f, 15.0f, 30.0f, 50.0f
};

static const float InitScale[HardCubeTypeMax] = {
	1.2f, 1.7f, 0.7f, 0.4f, 1.2f
};

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
	ptr->scale = InitScale[ptr->type];
	ptr->hp = InitHp[ptr->type];

	if (ptr->type != HardCubeBezierType)
		ChangeStateHardCube(ptr, HardCubeMove);
	else
		ChangeStateHardCube(ptr, HardCubeBezier);
}

/**************************************
更新処理
***************************************/
void OnUpdateHardCubeInit(HARD_CUBE_OBJECT *ptr)
{

}