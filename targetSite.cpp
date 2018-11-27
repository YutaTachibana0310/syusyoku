//=====================================
//
//テンプレート処理[targetSite.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "targetSite.h"
#include "playerModel.h"
#include "battleCamera.h"
#include "enemyMissile.h"
#include "debugproc.h"
#include "rockonSite.h"

/**************************************
マクロ定義
***************************************/
#define TARGETSITE_SIZE_X				(40.0f)
#define TARGETSITE_SIZE_Y				(40.0f)
#define TARGETSITE_TEXTURE				"data/TEXTURE/targetSite1.png"
#define TARGETSITE_MAX					PLAYERMODEL_MAX
#define TARGETSITE_MOVEVALUE			(3.0f)
#define TARGETSITE_INCIRCLE_ROTVALUE	(0.01f)
#define TARGETSITE_OUTCIRCLE_ROTVALUE	(-0.015f)
#define TARGETSITE_COLLIDER_RADIUS		(180.0f)

/**************************************
構造体定義
***************************************/
enum TARGETSITE_TEXTUREINDEX
{
	TARGETSITE_INCIRCLE,
	TARGETSITE_OUTCIRCLE,
	TARGETSITE_TEXTUREMAX
};
/**************************************
グローバル変数
***************************************/
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;
static LPDIRECT3DTEXTURE9 texture[TARGETSITE_TEXTUREMAX];
static D3DXMATRIX mtxWorld;

static TARGETSITE targetSite[TARGETSITE_MAX];

static const char* texturePath[] = {
	"data/TEXTURE/targetSite1_1.png",
	"data/TEXTURE/targetSite1_2.png"
};

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexTargetSite(void);

/**************************************
初期化処理
***************************************/
void InitTargetSite(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		for (int i = 0; i < TARGETSITE_TEXTUREMAX; i++)
		{
			texture[i] = CreateTextureFromFile((LPSTR)texturePath[i], pDevice);
		}

		MakeVertexTargetSite();
	}

	TARGETSITE *ptr = &targetSite[0];
	for (int i = 0; i < TARGETSITE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		ptr->insideRot = ptr->outsideRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

/**************************************
終了処理
***************************************/
void UninitTargetSite(int num)
{
	TARGETSITE *ptr = &targetSite[0];
	for (int i = 0; i < TARGETSITE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(9999.9f, 9999.9f, -9999.9f);
	}

	if (num == 0)
	{
		for (int j = 0; j < TARGETSITE_TEXTUREMAX; j++)
		{
			SAFE_RELEASE(texture[j]);
			SAFE_RELEASE(vtxBuff);
		}
	}
}

/**************************************
更新処理
***************************************/
void UpdateTargetSite(void)
{
	TARGETSITE *ptr = &targetSite[0];
	D3DXVECTOR3 tmpPos;
	VERTEX_3D *pVtx = NULL;

	//vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < TARGETSITE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//移動
		//D3DXVECTOR3 diff = ptr->targetPos - ptr->pos;
		//float length = D3DXVec3Length(&diff);
		//D3DXVec3Normalize(&diff, &diff);
		//length = Clampf(0.0f, TARGETSITE_MOVEVALUE, length);
		//ptr->pos += diff * length;

		////topLをプロジェクション変換
		//D3DXVec3TransformCoord(&ptr->topL, &(ptr->pos + pVtx[0].vtx), &GetBattleCameraView());
		//D3DXVec3TransformCoord(&ptr->topL, &ptr->topL, &GetBattleCameraProjection());

		////topRをプロジェクション変換
		//D3DXVec3TransformCoord(&ptr->topR, &(ptr->pos + pVtx[1].vtx), &GetBattleCameraView());
		//D3DXVec3TransformCoord(&ptr->topR, &ptr->topR, &GetBattleCameraProjection());

		////bottomLをプロジェクション変換
		//D3DXVec3TransformCoord(&ptr->bottomL, &(ptr->pos + pVtx[2].vtx), &GetBattleCameraView());
		//D3DXVec3TransformCoord(&ptr->bottomL, &ptr->bottomL, &GetBattleCameraProjection());

		////bottomRをプロジェクション変換
		//D3DXVec3TransformCoord(&ptr->bottomR, &(ptr->pos + pVtx[3].vtx), &GetBattleCameraView());
		//D3DXVec3TransformCoord(&ptr->bottomR, &ptr->bottomR, &GetBattleCameraProjection());

		//内側のサークルを回転
		ptr->insideRot.z += TARGETSITE_INCIRCLE_ROTVALUE;

		//外側のサークルを更新
		ptr->outsideRot.z += TARGETSITE_OUTCIRCLE_ROTVALUE;

		RockonEnemy(i);
	}

	//vtxBuff->Unlock();
}

/**************************************
描画処理
***************************************/
void DrawTargetSite(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate, mtxRot;

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

	TARGETSITE *ptr = &targetSite[0];
	for (int i = 0; i < TARGETSITE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//内側の円を描画
		D3DXMatrixIdentity(&mtxWorld);
		D3DXMatrixIdentity(&mtxTranslate);

		//GetInvRotBattleCamera(&mtxWorld);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, ptr->insideRot.y, ptr->insideRot.x, ptr->insideRot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		pDevice->SetTexture(0, texture[TARGETSITE_INCIRCLE]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

		//外側の円を描画
		D3DXMatrixIdentity(&mtxWorld);
		D3DXMatrixIdentity(&mtxTranslate);

		//GetInvRotBattleCamera(&mtxWorld);
		D3DXMatrixRotationYawPitchRoll(&mtxRot, ptr->outsideRot.y, ptr->outsideRot.x, ptr->outsideRot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		pDevice->SetTexture(0, texture[TARGETSITE_OUTCIRCLE]);
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexTargetSite(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vtxBuff,
		NULL)))
	{
		return;
	}

	VERTEX_3D *pVtx;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(-TARGETSITE_SIZE_X, TARGETSITE_SIZE_Y, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(TARGETSITE_SIZE_X, TARGETSITE_SIZE_Y, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-TARGETSITE_SIZE_X, -TARGETSITE_SIZE_Y, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(TARGETSITE_SIZE_X, -TARGETSITE_SIZE_Y, 0.0f);

	pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	vtxBuff->Unlock();

	return;
}

/**************************************
座標セット処理
***************************************/
void SetTargetSitePosition(D3DXVECTOR3 pos, int id)
{
	D3DXVECTOR3 screenPos;
	D3DXVec3TransformCoord(&screenPos, &pos, &GetBattleCameraView());
	D3DXVec3TransformCoord(&screenPos, &screenPos, &GetBattleCameraProjection());


	targetSite[id].pos = pos;
}

/**************************************
アドレス取得処理
***************************************/
TARGETSITE *GetTargetSiteAdr(int id)
{
	return &targetSite[id];
}

/**************************************
当たり判定
***************************************/
bool CollisionTargetSite(int id, const D3DXVECTOR3* pos, D3DXVECTOR3* siteScreenPos)
{
	D3DXVECTOR3 screenPos;
	TARGETSITE *ptr = &targetSite[id];
	PLAYERMODEL *player = GetPlayerAdr(id);

	D3DXVec3TransformCoord(&screenPos, pos, &GetBattleCameraView());
	D3DXVec3TransformCoord(&screenPos, &screenPos, &GetBattleCameraProjection());
	TranslateViewPort(&screenPos, &screenPos);

	//if (screenPos.x > ptr->topL.x && screenPos.x < ptr->topR.x)
	//{
	//	if (screenPos.y > ptr->bottomL.y && screenPos.y < ptr->topL.y)
	//	{
	//		return true;
	//	}
	//}

	if (D3DXVec3LengthSq(&(screenPos - *siteScreenPos)) < powf(TARGETSITE_COLLIDER_RADIUS, 2))
		return true;
	else
		return false;
}

/**************************************
エネミーロックオン判定
***************************************/
void RockonEnemy(int id)
{
	ENEMYMISSILE *enemy = GetEnemyMissileAdr(0);
	D3DXVECTOR3 siteScreenPos;

	if (GetPlayerAdr(id)->atkInterbal < PLAYER_HOMINGATK_INTERBAL)
	{
		return;
	}

	//座標をスクリーン座標に変換
	D3DXVec3TransformCoord(&siteScreenPos, &targetSite[id].pos, &GetBattleCameraView());
	D3DXVec3TransformCoord(&siteScreenPos, &siteScreenPos, &GetBattleCameraProjection());
	TranslateViewPort(&siteScreenPos, &siteScreenPos);

	//判定
	for (int i = 0; i < ENEMYMISSILE_MAX; i++, enemy++)
	{
		if (!enemy->active)
		{
			continue;
		}

		if (CollisionTargetSite(id, &enemy->pos, &siteScreenPos))
		{
			AddRockonTarget(id, &enemy->pos, &enemy->active, &enemy->hp);
		}
	}
}