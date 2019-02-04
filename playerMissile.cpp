//=====================================
//
//プレイヤーミサイル処理[playerMissile.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerMissile.h"
#include "enemyMissile.h"
#include "playerMissileSmog.h"
#include "particleManager.h"
#include "scoreMagniGUI.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERMISSILE_MODEL				"data/MODEL/missile.x"
#define PLAYERMISSILE_COLLIDER_RADIUS	(20.0f)
#define PLAYERMISSILE_REACEFRAME		(30.0f)
#define PLAYERMISSILE_VELOCITY_MAX		(15.0f)
#define PLAYERMISSILE_VELOCITY_MIN		(5.0f)
#define PLAYERMISSILE_DAMAGE			(500.0f)

/**************************************
構造体定義
***************************************/
typedef void(*funcPlayerMissile)(PLAYERMISSILE*);

/**************************************
グローバル変数
***************************************/
static LPD3DXMESH mesh;
static LPD3DXBUFFER materials;
static DWORD numMaterial;
static D3DXMATRIX mtxWorld;
static PLAYERMISSILE missile[PLAYERMISSILE_MAX];

//入場処理
static funcPlayerMissile Enter[PLAYERMISSILE_STATEMAX] = {
	EnterPlayerMissileLaunch,
	EnterPlayerMissileAccel,
	EnterPlayerMissileHoming,
	EnterPlayerMissileStraight
};

//更新処理
static funcPlayerMissile Update[PLAYERMISSILE_STATEMAX] = {
	UpdatePlayerMissileLaunch,
	UpdatePlayerMissileAccel,
	UpdatePlayerMissileHoming,
	UpdatePlayerMissileStraight
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitPlayerMissile(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		mesh = NULL;
		materials = NULL;

		if (FAILED(D3DXLoadMeshFromX(PLAYERMISSILE_MODEL,
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

		initialized = true;
	}

	PLAYERMISSILE *ptr = &missile[0];
	for (int i = 0; i < PLAYERMISSILE_MAX; i++, ptr++)
	{
		ptr->active = false;
		ptr->pos = D3DXVECTOR3(0.0f, 0.0f, -9999.9f);
		ptr->rot = D3DXQUATERNION(0.0f, 3.14f, 0.0f, 0.0f);
		//ptr->collider.active = false;
		//ptr->collider.radius = false;
	}
}

/**************************************
終了処理
***************************************/
void UninitPlayerMissile(int num)
{
	PLAYERMISSILE *ptr = &missile[0];
	for (int i = 0; i < PLAYERMISSILE_MAX; i++, ptr++)
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
void UpdatePlayerMissile(void)
{
	PLAYERMISSILE *ptr = &missile[0];
	D3DXVECTOR3 diff;			//ターゲット座標と自分の座標の差分
	D3DXVECTOR3 acceleration;	//加速度
	float period;

	for (int i = 0; i < PLAYERMISSILE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		Update[ptr->state](ptr);

		//スモッグセット
		SetPlayerMissileSmog(ptr->pos);
	}

	ColliisonPlayerMissileAndEnemyMissile();
}

/**************************************
描画処理
***************************************/
void DrawPlayerMissile(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxRot;
	D3DXMATERIAL *pMaterial;
	D3DMATERIAL9 matDef;

	pDevice->GetMaterial(&matDef);

	PLAYERMISSILE *ptr = &missile[0];

	for (int i = 0; i < PLAYERMISSILE_MAX; ptr++, i++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//Init World
		D3DXMatrixIdentity(&mtxWorld);

		//rotation
		D3DXMatrixRotationQuaternion(&mtxRot, &ptr->rot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		//translate
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		//set world
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		//set texture
		pDevice->SetTexture(0, NULL);

		//get material
		pMaterial = (D3DXMATERIAL*)materials->GetBufferPointer();

		//draw
		for (int j = 0; j < (int)numMaterial; j++)
		{
			pDevice->SetMaterial(&pMaterial[j].MatD3D);
			mesh->DrawSubset(j);
		}
	}

	//reset material
	pDevice->SetMaterial(&matDef);
}

/**************************************
ゲッター
***************************************/
PLAYERMISSILE *GetPlayerMissileAdr(int id)
{
	return &missile[id];
}

/**************************************
セッター
***************************************/
void SetPlayerMissile(D3DXVECTOR3 *pTargetPos, float *pTargetHP, bool *targetActive, D3DXVECTOR3 pos)
{
	PLAYERMISSILE *ptr = &missile[0];

	for (int i = 0; i < PLAYERMISSILE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		//ターゲット設定
		ptr->active = true;
		ptr->target = pTargetPos;
		ptr->targetHP = pTargetHP;
		ptr->targetActive = targetActive;

		//初速度設定
		float vX = (pTargetPos->x - pos.x) > 0 ? 5.0f : -5.0f;
		float vY = RandomRangef(-50.0f, 50.0f);
		float vZ = RandomRangef(-50.0f, -30.0f);
		ptr->velocity = D3DXVECTOR3(vX, vY,vZ);
		D3DXVec3Normalize(&ptr->velocity, &ptr->velocity);
		ptr->startSpeed = ptr->speed = RandomRangef(PLAYERMISSILE_VELOCITY_MIN, PLAYERMISSILE_VELOCITY_MAX);

		//パラメータ初期化
		ptr->cntFrame = 0;
		ptr->pos = pos;

		ChangeStatePlayerMissile(ptr, PLAYERMISSILE_LAUNCH);
		return;
	}
}

#if 1
/**************************************
当たり判定
***************************************/
void ColliisonPlayerMissileAndEnemyMissile(void)
{
	PLAYERMISSILE *ptr = &missile[0];
	float distSq, radiusSq;

	radiusSq = powf(10.0f, 2.0f);

	for(int i = 0; i < PLAYERMISSILE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		if (!ptr->targetActive)
		{
			continue;
		}


		distSq = D3DXVec3LengthSq(&(*ptr->target - ptr->pos));
		if (distSq < radiusSq)
		{
			*ptr->targetHP -= PLAYERMISSILE_DAMAGE;
			SetEnemyExplosion(ptr->pos);
			ptr->active = false;
		}
	}
}
#endif

/**************************************
状態遷移処理
***************************************/
void ChangeStatePlayerMissile(PLAYERMISSILE *ptr, int next)
{
	ptr->state = next;
	Enter[ptr->state](ptr);
}