//=====================================
//
//スモールエネミー処理[smallEnemyModel.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "smallEnemyModel.h"
#include "targetSite.h"
#include "playerModel.h"
#include "particleManager.h"
#include "playerBullet.h"

/**************************************
マクロ定義
***************************************/
#define SMALLENEMY_COLLIDER_RADIUS		(5.0f);
#define SMALLENEMY_MODEL_NAME			"data/MODEL/smallEnemy.x"
#define SMALLENEMY_TEXTURE_MAX			(14)
#define SMALLENEMY_MODEL_NAME_DEBUG		"data/MODEL/airplane000.x"

/**************************************
構造体定義
***************************************/
typedef void(*funcSmallEnemy)(SMALLENEMY *enemy);

/**************************************
グローバル変数
***************************************/
static LPD3DXMESH mesh = NULL;
static LPD3DXBUFFER materials = NULL;
static DWORD numMaterial = 0;
static SMALLENEMY smallEnemy[SMALLENEMY_MAX];

//更新処理テーブル
static funcSmallEnemy Update[SmallEnemyStateMax] = {
	UpdateSmallEnemyMove,
	UpdateSmallEnemyAttack,
	UpdateSmallEnemyEscape,
	UpdateSmallEnemyWait
};

//入場処理テーブル
static funcSmallEnemy Enter[SmallEnemyStateMax] = {
	EnterSmallEnemyMove,
	EnterSmallEnemyAttack,
	EnterSmallEnemyEscape,
	EnterSmallEnemyWait
};

//退場処理テーブル
static funcSmallEnemy Exit[SmallEnemyStateMax] = {
	ExitSmallEnemyMove,
	ExitSmallEnemyAttack,
	ExitSmallEnemyEscape,
	ExitSmallEnemyWait
};

static LPDIRECT3DTEXTURE9 textures[SMALLENEMY_TEXTURE_MAX];

static const char* textureName[SMALLENEMY_TEXTURE_MAX] = {
	NULL,
	NULL,
	"data/TEXTURE/ENEMY/cp2.jpg",
	"data/TEXTURE/ENEMY/vctu.jpg",
	"data/TEXTURE/ENEMY/vctu.jpg",
	NULL,
	"data/TEXTURE/ENEMY/ggt1.jpg",
	"data/TEXTURE/ENEMY/yx1_02_01_01.jpg",
	"data/TEXTURE/ENEMY/s1.jpg",
	"data/TEXTURE/ENEMY/as1_02.jpg",
	NULL,
	NULL,
	"data/TEXTURE/ENEMY/vctu.jpg",
	"data/TEXTURE/ENEMY/mnd1.jpg",
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitSmallEnemy(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

#ifdef _DEBUG
	if (num == 0)
	{
		D3DXLoadMeshFromX(SMALLENEMY_MODEL_NAME_DEBUG,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&materials,
			NULL,
			&numMaterial,
			&mesh);
	}
#else
	if (num == 0)
	{
		if (FAILED(D3DXLoadMeshFromX(SMALLENEMY_MODEL_NAME,
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&materials,
			NULL,
			&numMaterial,
			&mesh)))
		{
			return;
		}

		for (int i = 0; i < SMALLENEMY_TEXTURE_MAX; i++)
		{
			textures[i] = CreateTextureFromFile((LPSTR)textureName[i], pDevice);
		}
	}
#endif
	SMALLENEMY *ptr = &smallEnemy[0];
	for (int i = 0; i < SMALLENEMY_MAX; i++, ptr++)
	{
		ptr->hp = 1.0f;
		ptr->collider.pos = &ptr->pos;
		ptr->collider.offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->collider.radius = SMALLENEMY_COLLIDER_RADIUS;
		ptr->active = false;
	}
}

/**************************************
終了処理
***************************************/
void UninitSmallEnemy(int num)
{
	SMALLENEMY *ptr = &smallEnemy[0];

	for (int i = 0; i < SMALLENEMY_MAX; i++, ptr++)
	{
		ptr->active = false;
	}

	if (num == 0)
	{
		SAFE_RELEASE(mesh);
		SAFE_RELEASE(materials);

		for (int i = 0; i < SMALLENEMY_TEXTURE_MAX; i++)
		{
			SAFE_RELEASE(textures[i]);
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateSmallEnemy(void)
{
	SMALLENEMY *ptr = &smallEnemy[0];
	for (int i = 0; i < SMALLENEMY_MAX; i++, ptr++)
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
void DrawSmallEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxWorld;
	D3DXMATERIAL *pMaterial;
	D3DMATERIAL9 matDef;

	pDevice->GetMaterial(&matDef);

	SMALLENEMY *ptr = &smallEnemy[0];
	for (int i = 0; i < SMALLENEMY_MAX; i++, ptr++)
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

#ifdef _DEBUG
	ptr = &smallEnemy[0];
	for (int i = 0; i < SMALLENEMY_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		DrawBoundingSphere(&ptr->collider);
	}
#endif

	pDevice->SetMaterial(&matDef);
}

/**************************************
状態遷移
***************************************/
void ChangeStateSmallEnemy(SMALLENEMY *ptr, int next)
{
	Exit[ptr->state](ptr);
	ptr->state = next;
	Enter[ptr->state](ptr);
}

/**************************************
アドレス取得処理
***************************************/
SMALLENEMY *GetSmallEnemyAdr(int id)
{
	return &smallEnemy[id];
}

/**************************************
スモールエネミーロックオン処理
***************************************/
void LockonSmallEnemy(void)
{
	SMALLENEMY *ptr = &smallEnemy[0];
	TARGETSITE *targetSite = GetTargetSiteAdr(0);

	for (int i = 0; i < PLAYERMODEL_MAX; i++, targetSite++)
	{
		if (!targetSite->active)
		{
			continue;
		}

		ptr = &smallEnemy[0];
		for (int j = 0; j < SMALLENEMY_MAX; j++, ptr++)
		{
			if (!ptr->active)
			{
				continue;
			}

			if (CollisionTargetSite(i, &ptr->pos))
			{
				AddRockonTarget(i, &ptr->pos, &ptr->active, &ptr->hp);
			}
		}
	}
}

/**************************************
バレット当たり判定
***************************************/
void CollisionSmallEnemyAndBullet(void)
{
	SMALLENEMY *ptr = &smallEnemy[0];
	PLAYERBULLET *bullet = GetPlayerBulletAdr(0);

	for (int i = 0; i < SMALLENEMY_MAX; i++, ptr++)
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