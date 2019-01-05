//=====================================
//
//エネミーミサイル処理[enemyMissile.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "enemyMissile.h"
#include "enemyMissileLaunch.h"
#include "enemyMissileHoming.h"
#include "enemyMissileStraight.h"
#include "enemyMissileSmog.h"
#include "playerBullet.h"
#include "playerModel.h"
#include "targetSite.h"
#include "particleManager.h"

#if 1
#include "battleCamera.h"
#include "input.h"
#include "particle.h"
#endif

#include "debugWindow.h"

/**************************************
マクロ定義
**************************************/
#define ENEMYMISSILE_MODEL				"data/MODEL/missile.x"		//モデル名
#define ENEMYMISSILE_ROTATEVALUE		(0.017f)					//1フレームあたりの回転量
#define ENEMYMISSILE_COLLIDER_RADIUS	(20.0f)				//当たり判定の半径
#define ENEMYMISSILE_DAMAGE				(0.25f)				//ミサイルのダメージ
/**************************************
構造体定義
**************************************/
typedef void(*funcEnemyMissile)(ENEMYMISSILE*);

/*************************************
プロトタイプ宣言
**************************************/
void CheckDestroyEnemyMissile(void);						//死亡判定

#ifdef USE_DEBUGWINDOW
void DrawDebugInfoEnemyMissile(void);
#endif

/*************************************
グローバル変数
**************************************/
static LPD3DXMESH mesh;										//メッシュへのポインタ
static LPD3DXBUFFER materials;								//マテリアル情報
static DWORD numMaterial;									//属性情報の総数
static D3DXMATRIX worldMtx;									//ワールドマトリクス
static ENEMYMISSILE missile[ENEMYMISSILE_MAX];				//エネミーミサイル構造体

static int cntFrame = 0;

//更新処理の関数テーブル
static funcEnemyMissile Update[ENEMYMISSILE_STATEMAX] = { 
	EnemyMissileLaunchUpdate,
	EnemyMissileHomingUpdate,
	EnemyMissileStraightUpdate
};

//入場処理の関数テーブル
static funcEnemyMissile Enter[ENEMYMISSILE_STATEMAX] = {
	EnemyMissileLaunchEnter,
	EnemyMissileHomingEnter,
	EnemyMissileStraightEnter,
};

//退場処理の関数テーブル
static funcEnemyMissile Exit[ENEMYMISSILE_STATEMAX] = {
	EnemyMissileLaunchExit,
	EnemyMissileHomingExit,
	EnemyMissileStraightExit
};

static LARGE_INTEGER startSet, endSet;
static LARGE_INTEGER startUpdate, endUpdate;
static LARGE_INTEGER startParticle, endParticle;
static LARGE_INTEGER startCollision, endCollision;

/*************************************
初期化処理
**************************************/
void InitEnemyMissile(int num)
{
	if (num == 0)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		mesh = NULL;
		materials = NULL;

		if (FAILED(D3DXLoadMeshFromX(ENEMYMISSILE_MODEL,
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
	}

	ENEMYMISSILE *ptr = &missile[0];
	for (int i = 0; i < ENEMYMISSILE_MAX; i++, ptr++)
	{
		ptr->active = false;
		ptr->pos = D3DXVECTOR3(0.0f, 0.0f, -9999.0f);
		ptr->rot = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 0.0f);
		ptr->velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->speed = 0.0f;
		ptr->collider.active = false;
		ptr->collider.radius = ENEMYMISSILE_COLLIDER_RADIUS;
	}
}

/****************************************
終了処理
*****************************************/
void UninitEnemyMissile(int num)
{
	ENEMYMISSILE *ptr = &missile[0];
	for (int i = 0; i < ENEMYMISSILE_MAX; i++)
	{
		missile[0].active = false;
	}

	if(num == 0)
	{
		SAFE_RELEASE(mesh);
		SAFE_RELEASE(materials);
	}
}

/****************************************
更新処理
*****************************************/
void UpdateEnemyMissile(void)
{

	ENEMYMISSILE *ptr = &missile[0];

	GetTimerCount(&startSet);
	cntFrame++;
	if (cntFrame % 2 == 0)
	{
		for (int k = 0; k < 16; k++)
		{
			float angle = RandomRangef(45.0f, 135.0f);
			D3DXVECTOR3 target = D3DXVECTOR3(cosf(0.017f * angle), sinf(0.017f * angle), 0.0f);
			//SetEnemyMissile(D3DXVECTOR3(RandomRange(-500.0f, 500.0f), -200.0f, 2000), target, GetBattleCameraPos() + D3DXVECTOR3(RandomRange(-20.0f, 20.0f), RandomRange(-20.0f, 20.0f), 0.0f));
		}
	}
	GetTimerCount(&endSet);

	GetTimerCount(&startUpdate);
	for (int i = 0; i < ENEMYMISSILE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		Update[ptr->state](ptr);
		ptr->cntFrame++;
	}
	GetTimerCount(&endUpdate);

	GetTimerCount(&startParticle);
	CheckDestroyEnemyMissile();
	GetTimerCount(&endParticle);

	GetTimerCount(&startCollision);
	//CollisionEnemyMissileAndBullet();
	GetTimerCount(&endCollision);


	GetTimerCount(&endUpdate);
}

/*****************************************
描画処理
******************************************/
void DrawEnemyMissile(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pMaterial;
	D3DMATERIAL9 matDef;

	pDevice->GetMaterial(&matDef);

	ENEMYMISSILE *ptr = &missile[0];
	for (int i = 0; i < ENEMYMISSILE_MAX; i++, ptr++)
	{
		if (!ptr->active)

		{
			continue;
		}

		//ワールドマトリクスの初期化
		D3DXMatrixIdentity(&worldMtx);

		//回転を反映
		D3DXMatrixRotationQuaternion(&mtxRot, &ptr->rot);
		D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxRot);

		//移動を反映
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&worldMtx, &worldMtx, &mtxTranslate);

		//ワールドマトリクスの設定
		pDevice->SetTransform(D3DTS_WORLD, &worldMtx);

		//テクスチャの設定（NULL）
		pDevice->SetTexture(0, NULL);

		//マテリアル情報に対するポインタを取得
		pMaterial = (D3DXMATERIAL*)materials->GetBufferPointer();

		//描画
		for (int i = 0; i < (int)numMaterial; i++)
		{
			pDevice->SetMaterial(&pMaterial[i].MatD3D);
			mesh->DrawSubset(i);
		}
	}

	//マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);

	DrawDebugInfoEnemyMissile();
}

/******************************************
エネミーミサイルアドレス取得関数
*******************************************/
ENEMYMISSILE *GetEnemyMissileAdr(int n)
{
	return (n < ENEMYMISSILE_MAX) ? &missile[n] : NULL;
}

/*****************************************
エネミーミサイル状態遷移関数
******************************************/
void ChangeStateEnemyMissile(ENEMYMISSILE *ptr, int targetState)
{
	//退場処理を呼び出し
	Exit[ptr->state](ptr);

	//状態遷移
	ptr->state = targetState;

	//入場処理を呼び出し
	Enter[ptr->state](ptr);
}

/*****************************************
エネミーミサイルセット関数
******************************************/
void SetEnemyMissile(D3DXVECTOR3 pos, D3DXVECTOR3 moveDir, D3DXVECTOR3 targetPos)
{
	ENEMYMISSILE *ptr = &missile[0];

	for (int i = 0; i < ENEMYMISSILE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->velocity = moveDir;
		ptr->cntFrame = 0;
		//ptr->state = ENEMYMISSILE_LAUNCH;
		ChangeStateEnemyMissile(ptr, ENEMYMISSILE_LAUNCH);
		ptr->hp = 1.0f;
		ptr->active = true;

		return;
	}
}

/*****************************************
エネミーミサイル当たり判定
******************************************/
void CollisionEnemyMissileAndBullet(void)
{
	ENEMYMISSILE *ptr = &missile[0];
	PLAYERBULLET *bullet = GetPlayerBulletAdr(0);
	float distSq, radiusSq;

	//プレイヤーバレットとの当たり判定
	for (int i = 0; i < ENEMYMISSILE_MAX; i++, ptr++)
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

			distSq = D3DXVec3LengthSq(&(bullet->pos - ptr->pos));
			if (distSq < 400.0f)
			{
				ptr->hp = 0.0f;
				bullet->active = false;
			}
		}
	}
}

/*****************************************
エネミーミサイルロックオン判定
******************************************/
void LockonEnemyMissile(void)
{
	ENEMYMISSILE *ptr = &missile[0];
	TARGETSITE *targetSite = GetTargetSiteAdr(0);

	for (int i = 0; i < PLAYERMODEL_MAX; i++, targetSite++)
	{
		if (!targetSite->active)
		{
			continue;
		}

		ptr = &missile[0];
		for (int j = 0; j < ENEMYMISSILE_MAX; j++, ptr++)
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

/*****************************************
死亡判定
******************************************/
void CheckDestroyEnemyMissile(void)
{
	ENEMYMISSILE *ptr = &missile[0];
	for (int i = 0; i < ENEMYMISSILE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		if (ptr->hp <= 0.0f)
		{
			//for (int j = 0; j < 2; j++)
			//{
			//	SetExplosionFlare(&ptr->pos);
			//}
			//for (int j = 0; j < 1; j++)
			//{
			//	SetExplosionFire(&ptr->pos);
			//}
			//for (int j = 0; j < 1; j++)
			//{
			//	SetExplosionSmog(&ptr->pos);
			//}
			SetEnemyExplosion(ptr->pos);
			ptr->active = false;

		}
	}
}

#ifdef USE_DEBUGWINDOW
/*****************************************
デバッグ情報表示
******************************************/
void DrawDebugInfoEnemyMissile(void)
{
	ImGui::StyleColorsDark();
	ImGui::SetNextWindowSize(ImVec2(200.0f, 100.0f));
	ImGui::SetNextWindowPos(ImVec2(5.0f, 310.0f));
	ImGui::Begin("EnemyMissle");

	ImGui::Text("Set       : %fmsec", CalcProgressTime(startSet, endSet));
	ImGui::Text("Update    : %fmsec", CalcProgressTime(startUpdate, endUpdate));
	ImGui::Text("Destroy   : %fmsec", CalcProgressTime(startParticle, endParticle));
	ImGui::Text("Collision : %fmsec", CalcProgressTime(startCollision, endCollision));

	ImGui::End();
}
#endif