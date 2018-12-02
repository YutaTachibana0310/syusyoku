//=====================================
//
//ミドルエネミーモデル処理[middleEnemyModel.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "middleEnemyModel.h"

/**************************************
マクロ定義
***************************************/
#define MIDDLEENMY_MODEL_NAME		"data/MODEL/MiddileEnemy.x"

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPD3DXMESH mesh = NULL;
static LPD3DXBUFFER materials = NULL;
static DWORD numMaterial = 0;
static MIDDLEENMYMODEL middleEnemy[MIDDLEENEMY_MAX];

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitMiddleEnemyModel(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		D3DXLoadMeshFromX(MIDDLEENMY_MODEL_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &materials, NULL, &numMaterial, &mesh);
	}

	MIDDLEENMYMODEL *ptr = &middleEnemy[0];
	for (int i = 0; i < MIDDLEENEMY_MAX; i++, ptr++)
	{
	

	}
}

/**************************************
終了処理
***************************************/
void UninitMiddleEnemyModel(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdateMiddleEnemyModel(void)
{

}

/**************************************
描画処理
***************************************/
void DrawMiddleEnemyModel(void)
{

}
