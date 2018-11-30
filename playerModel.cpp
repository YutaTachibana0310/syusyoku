//=====================================
//
//プレイヤーモデル処理[playerModel.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModel.h"
#include "input.h"
#include "playerModelFPS.h"
#include "playerModelTopView.h"
#include "playerModelSideView.h"
#include "playerModelQuaterView.h"
#include "playerBullet.h"

#ifdef _DEBUG
#include "debugproc.h"
#endif

/**************************************
マクロ定義
***************************************/
#define PLAYERMODEL_MODELNAME		"data/MODEL/airplane000.x"
#define PLAYER_ROTATEMAGNI			(0.2f)
#define PLATER_ROTATEVALUE_MAX		(0.085f)

/**************************************
構造体定義
***************************************/
typedef void(*funcPlayerModel)(PLAYERMODEL *player);

/**************************************
グローバル変数
***************************************/
static PLAYERMODEL model[PLAYERMODEL_MAX];
static LPD3DXMESH mesh = NULL;
static LPD3DXBUFFER materials = NULL;
static DWORD numMaterial = 0;

static int playerState;

static funcPlayerModel Update[PlayerStateMax] = {
	UpdatePlayerModelFPS,
	UpdatePlayerModelTopView,
	UpdatePlayerModelSideView,
	UpdatePlayerModelQuaterView
};

static funcPlayerModel Enter[PlayerStateMax] = {
	EnterPlayerModelFPS,
	EnterPlayerModelTopView,
	EnterPlayerModelSideView,
	EnterPlayerModelQuaterView
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitPlayerModel(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	if (num == 0)
	{
		D3DXLoadMeshFromX(PLAYERMODEL_MODELNAME, D3DXMESH_SYSTEMMEM, pDevice, NULL, &materials, NULL, &numMaterial, &mesh);
	}

	PLAYERMODEL *ptr = &model[0];
	for (int i = 0; i < PLAYERMODEL_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(0, 0, 0);
		
		ptr->rot = ptr->destRot = D3DXVECTOR3(0, D3DXToRadian(180.0f), 0);
		ptr->scale = D3DXVECTOR3(10.0f, 10.0f, 10.0f);

		ptr->id = i;
	}

	ChangeStatePlayerModel(PlayerFPS);
	model[0].active = true;
}

/**************************************
終了処理
***************************************/
void UninitPlayerModel(int num)
{
	PLAYERMODEL *ptr = &model[0];
	for (int i = 0; i < PLAYERMODEL_MAX; i++, ptr++)
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
void UpdatePlayerModel(void)
{
	PLAYERMODEL *ptr = &model[0];

	for (int i = 0; i < PLAYERMODEL_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		Update[playerState](ptr);

		D3DXVECTOR3 diff = ptr->destRot - ptr->rot;
		ptr->rot.x += Clampf(-PLATER_ROTATEVALUE_MAX, PLATER_ROTATEVALUE_MAX, diff.x * PLAYER_ROTATEMAGNI);
		ptr->rot.y += Clampf(-PLATER_ROTATEVALUE_MAX, PLATER_ROTATEVALUE_MAX, diff.y * PLAYER_ROTATEMAGNI);
		ptr->rot.z += Clampf(-PLATER_ROTATEVALUE_MAX, PLATER_ROTATEVALUE_MAX, diff.z * PLAYER_ROTATEMAGNI);

		PrintDebugProc("playerpos[%d]:%f %f %f\n", i, ptr->pos.x, ptr->pos.y, ptr->pos.z);
	}


}

/**************************************
描画処理
***************************************/
void DrawPlayerModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate, mtxScale, mtxWorld;
	D3DXMATERIAL *pMaterial;
	D3DMATERIAL9 matDef;

	pDevice->GetMaterial(&matDef);
	pDevice->SetTexture(0, NULL);	

	PLAYERMODEL *ptr = &model[0];
	for (int i = 0; i < PLAYERMODEL_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//ワールドマトリクスの変換
		D3DXMatrixIdentity(&mtxWorld);
		D3DXMatrixScaling(&mtxScale, ptr->scale.x, ptr->scale.y, ptr->scale.z);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, ptr->rot.y, ptr->rot.x, ptr->rot.z);
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);

		//D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScale);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		pMaterial = (D3DXMATERIAL*)materials->GetBufferPointer();

		for (int j = 0; j < (int)numMaterial; j++)
		{
			pDevice->SetMaterial(&pMaterial[j].MatD3D);
			mesh->DrawSubset(j);
		}
	}

	pDevice->SetMaterial(&matDef);
}

/*********************************************
関数名	：PLAYERMODEL *GetPlayerModelAdr(int num)
引数	：int num … アドレスを取得したいプレイヤーのインデックス
戻り値	：プレイヤー構造体のアドレス
*********************************************/
PLAYERMODEL *GetPlayerAdr(int num)
{
	return &model[num];
}

/*********************************************
関数名	：PLAYERMODEL *GetPlayerModelAdr(int num)
引数	：int next 次の状態
戻り値	：プレイヤー構造体のアドレス
*********************************************/
void ChangeStatePlayerModel(int next)
{

	playerState = next;
	PLAYERMODEL *ptr = &model[0];
	for (int i = 0; i < PLAYERMODEL_MAX; i++, ptr++)
	{
		if (!ptr->active)
			continue;

		Enter[playerState](ptr);
	}
}

/*********************************************
関数名	：void AddRockonTarget(int id, D3DXVECTOR3 *targetPos, bool *targetActive, float *targetHP)
引数	：int id：ロックオンしたプレイヤーのID
引数	：D3DXVECTOR3 *targetPos：ロックオン対象の座標
引数	：bool *targetActive：ロックオン対象のアクティブ状態
引数	：float *targetHP：ロックオン対象のHP
戻り値	：void
*********************************************/
ROCKONTARGET *AddRockonTarget(int id, D3DXVECTOR3 *targetPos, bool *targetActive, float *targetHP)
{
	PLAYERMODEL *ptr = &model[id];

	//インターバル判定
	if (ptr->atkInterbal < PLAYER_HOMINGATK_INTERBAL)
	{
		return NULL;
	}

	//ダブリ判定
	for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
	{
		if (ptr->target[i].pos == targetPos)
		{
			return NULL;
		}
	}

	//未使用のターゲットに設定
	for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
	{
		//ロックオンの空きがあるまでループ
		if (ptr->target[i].use)
		{
			continue;
		}

		//ロックオン対象に設定しreturn
		ptr->target[i].pos = targetPos;
		ptr->target[i].active = targetActive;
		ptr->target[i].hp = targetHP;
		ptr->target[i].use = true;
		ActivateRockonSite(ptr->id * PLAYER_ROCKON_MAX + i);
		ptr->lockonNum++;
		return &ptr->target[i];
	}

	//ロックオンの空きがないのでreturn NULL
	return NULL;
}

/*********************************************
関数名	：ReleaseRockonTarget(ROCKONTARGET *target)
引数	：ROCKONTARGET *target：ロックオンを解除したいターゲットへのポインタ
戻り値	：void
*********************************************/
void ReleaseRockonTarget(PLAYERMODEL *player, int targetID)
{
	//target->active = NULL;
	//target->hp = NULL;
	//target->pos = NULL;

	player->target[targetID].active = NULL;
	player->target[targetID].hp = NULL;
	player->target[targetID].pos = NULL;
	player->target[targetID].use = false;
	player->lockonNum--;
}