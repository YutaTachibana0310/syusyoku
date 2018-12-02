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
#define MIDDLEENEMY_MODEL_NAME		"data/MODEL/middleEnemy.x"
#define MIDDLEENEMY_TEXTURE_MAX		(11)

/**************************************
構造体定義
***************************************/
typedef void(*funcMiddleEnemy)(MIDDLEENEMYMODEL *enemy);

/**************************************
グローバル変数
***************************************/
static LPD3DXMESH mesh = NULL;
static LPD3DXBUFFER materials = NULL;
static DWORD numMaterial = 0;
static MIDDLEENEMYMODEL middleEnemy[MIDDLEENEMY_MAX];
static LPDIRECT3DTEXTURE9 textures[MIDDLEENEMY_TEXTURE_MAX];

static const char* textureName[MIDDLEENEMY_TEXTURE_MAX] = {
	"data/TEXTURE/ENEMY/cockpit_mtl3_diffcol.jpg",
	"data/TEXTURE/ENEMY/cockpit_mtl1_diffcol.jpg",
	"data/TEXTURE/ENEMY/sd1.jpg",
	"data/TEXTURE/ENEMY/c114_07_01_01_01_01_01_01_01_03.jpg",
	NULL,
	NULL,
	"data/TEXTURE/ENEMY/c2b_03_01.jpg",
	NULL,
	"data/TEXTURE/ENEMY/jp1.jpg",
	"data/TEXTURE/ENEMY/bldg6_Form22_1_mtl1_diffcol.jpg",
	"data/TEXTURE/ENEMY/cockpit_mtl2_diffcol.jpg",
};

//更新処理の関数テーブル
static funcMiddleEnemy Update[MiddleEnemyStateMax] = {
	UpdateMiddleEnemyMove,
};

//入場処理の関数テーブル
static funcMiddleEnemy Enter[MiddleEnemyStateMax] = {
	EnterMiddleEnemyMove,
};

//退場処理の関数テーブル
static funcMiddleEnemy Exit[MiddleEnemyStateMax] = {
	ExitMiddleEnemyMove,
};

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
		HRESULT res = D3DXLoadMeshFromX(MIDDLEENEMY_MODEL_NAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &materials, NULL, &numMaterial, &mesh);
		if (FAILED(res))
		{
			return;
		}

		for (int i = 0; i < MIDDLEENEMY_TEXTURE_MAX; i++)
		{
			textures[i] = CreateTextureFromFile((LPSTR)textureName[i], pDevice);
		}
	}

	MIDDLEENEMYMODEL *ptr = &middleEnemy[0];
	for (int i = 0; i < MIDDLEENEMY_MAX; i++, ptr++)
	{
		ptr->active = false;
	}

	middleEnemy[0].active = true;
	middleEnemy[0].pos = D3DXVECTOR3(50.0f, -50.0f, -200.0f);
	middleEnemy[0].goalPos = D3DXVECTOR3(50.0f, -0.0f, 100.0f);
	middleEnemy[0].rot = D3DXVECTOR3(0.0f, 3.14f, 0.0f);
	middleEnemy[0].goalRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ChangeStateMiddleEnemy(&middleEnemy[0], MiddleEnemyMove);
}

/**************************************
終了処理
***************************************/
void UninitMiddleEnemyModel(int num)
{
	MIDDLEENEMYMODEL *ptr = &middleEnemy[0];

	for (int i = 0; i < MIDDLEENEMY_MAX; i++, ptr++)
	{
		ptr->active = false;
	}

	if (num == 0)
	{
		SAFE_RELEASE(mesh);
		SAFE_RELEASE(materials);
	}
}

/**************************************
更新処理
***************************************/
void UpdateMiddleEnemyModel(void)
{
	MIDDLEENEMYMODEL *ptr = &middleEnemy[0];
	for (int i = 0; i < MIDDLEENEMY_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		Update[ptr->state](ptr);
	}
}

/**************************************
描画処理
***************************************/
void DrawMiddleEnemyModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
	D3DXMATERIAL *pMaterial;
	D3DMATERIAL9 matDef;

	pDevice->GetMaterial(&matDef);

	MIDDLEENEMYMODEL *ptr = &middleEnemy[0];
	for (int i = 0; i < MIDDLEENEMY_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		D3DXMatrixIdentity(&mtxWorld);

		//rotate
		D3DXMatrixRotationYawPitchRoll(&mtxRot, ptr->rot.y, ptr->rot.x, ptr->rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//translate
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		//set world
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//draw
		pMaterial = (D3DXMATERIAL*)materials->GetBufferPointer();
		for (int j = 0; j < (int)numMaterial; j++)
		{
			pDevice->SetMaterial(&pMaterial[j].MatD3D);
			pDevice->SetTexture(0, textures[j]);
			mesh->DrawSubset(j);
		}
	}

	pDevice->SetMaterial(&matDef);
}

/**************************************
状態遷移処理
***************************************/
void ChangeStateMiddleEnemy(MIDDLEENEMYMODEL *ptr, int next)
{
	Exit[ptr->state](ptr);
	ptr->state = next;
	Enter[ptr->state](ptr);
}