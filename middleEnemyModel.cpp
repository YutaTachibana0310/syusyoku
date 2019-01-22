//=====================================
//
//ミドルエネミーモデル処理[MIDDLEENEMY.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "middleEnemyModel.h"
#include "targetSite.h"
#include "playerModel.h"
#include "particleManager.h"
#include "playerBullet.h"

/**************************************
マクロ定義
***************************************/
#define MIDDLEENEMY_MODEL_NAME		"data/MODEL/middleEnemy.x"
#define MIDDLEENEMY_TEXTURE_MAX		(11)
#define MIDDLEENEMY_MODEL_NAME_DEBUG	"data/MODEL/airplane000.x"
/**************************************
構造体定義
***************************************/
typedef void(*funcMiddleEnemy)(MIDDLEENEMY *enemy);

/**************************************
グローバル変数
***************************************/
static LPD3DXMESH mesh = NULL;
static LPD3DXBUFFER materials = NULL;
static DWORD numMaterial = 0;
static MIDDLEENEMY middleEnemy[MIDDLEENEMY_MAX];
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
	UpdateMiddleEnemyAttack,
	UpdateMiddleEnemyEscape,
	UpdateMiddleEnemyWait
};

//入場処理の関数テーブル
static funcMiddleEnemy Enter[MiddleEnemyStateMax] = {
	EnterMiddleEnemyMove,
	EnterMiddleEnemyAttack,
	EnterMiddleEnemyEscape,
	EnterMiddleEnemyWait
};

//退場処理の関数テーブル
static funcMiddleEnemy Exit[MiddleEnemyStateMax] = {
	ExitMiddleEnemyMove,
	ExitMiddleEnemyAttack,
	ExitMiddleEnemyEscape,
	ExitMiddleEnemyWait
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitMiddleEnemy(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static bool initialized = false;
#ifdef _DEBUG
	if (!initialized)
	{
		D3DXLoadMeshFromX(MIDDLEENEMY_MODEL_NAME_DEBUG,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&materials,
			NULL,
			&numMaterial,
			&mesh);

		initialized = true;
	}
#else
	if (!initialized)
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

		initialized = true;
	}
#endif
	MIDDLEENEMY *ptr = &middleEnemy[0];
	for (int i = 0; i < MIDDLEENEMY_MAX; i++, ptr++)
	{
		ptr->hp = 2.0f;
		ptr->collider.pos = &ptr->pos;
		ptr->collider.radius = MIDDLEENEMY_COLLIDER_RADIUS;
		ptr->collider.offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->active = false;

	}
}

/**************************************
終了処理
***************************************/
void UninitMiddleEnemy(int num)
{
	MIDDLEENEMY *ptr = &middleEnemy[0];

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
void UpdateMiddleEnemy(void)
{
	MIDDLEENEMY *ptr = &middleEnemy[0];
	for (int i = 0; i < MIDDLEENEMY_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//撃墜判定
		if (ptr->hp <= 0.0f)
		{
			SetEnemyExplosion(ptr->pos);
			ptr->active = false;
		}

		//各状態の更新処理
		Update[ptr->state](ptr);

		
	}
}

/**************************************
描画処理
***************************************/
void DrawMiddleEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
	D3DXMATERIAL *pMaterial;
	D3DMATERIAL9 matDef;

	pDevice->GetMaterial(&matDef);

	MIDDLEENEMY *ptr = &middleEnemy[0];
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

#ifdef _DEBUG
		DrawBoundingSphere(&ptr->collider);
#endif
	}

	pDevice->SetMaterial(&matDef);
}

/**************************************
状態遷移処理
***************************************/
void ChangeStateMiddleEnemy(MIDDLEENEMY *ptr, int next)
{
	Exit[ptr->state](ptr);
	ptr->state = next;
	Enter[ptr->state](ptr);
}

/**************************************
アドレス取得処理
***************************************/
MIDDLEENEMY *GetMiddleEnemyAdr(int num)
{
	return &middleEnemy[num];
}

/**************************************
ミドルエネミーロックオン処理
***************************************/
void LockonMiddleEnemy(void)
{
	MIDDLEENEMY *ptr = &middleEnemy[0];
	TARGETSITE *targetSite = GetTargetSiteAdr(0);

	for(int i = 0; i < PLAYERMODEL_MAX; i++, targetSite++)
	{
		if (!targetSite->active)
		{
			continue;
		}

		ptr = &middleEnemy[0];
		for (int j = 0; j < MIDDLEENEMY_MAX; j++, ptr++)
		{
			if (!ptr->active)
			{
				continue;
			}

			if (CollisionTargetSite(i, &ptr->pos))
			{
				AddRockonTarget(i, &ptr->pos,&ptr->active, &ptr->hp);
			}
		}

	}
}

/**************************************
ミドルエネミーのバレット当たり判定
***************************************/
void CollisionMiddleEnemyAndBullet(void)
{
	MIDDLEENEMY *ptr = &middleEnemy[0];
	PLAYERBULLET *bullet = GetPlayerBulletAdr(0);

	for (int i = 0; i < MIDDLEENEMY_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		bullet = GetPlayerBulletAdr(0);

		for (int j = 0; j < PLAYERBULLET_MAX; j++, bullet++)
		{
			if (!bullet->active)
			{
				continue;
			}

			if (CheckHitBoundingSphere(&ptr->collider, &bullet->collider))
			{
				ptr->hp -= 1.0f;
				bullet->active = false;
			}

		}
	}
}