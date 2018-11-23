//=====================================
//
//テンプレート処理[playerMissileSmog.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerMissileSmog.h"
#include "battleCamera.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERMISSILESMOG_TEXSIZE		(10)
#define PLAYERMISSILESMOG_TEXTURE		"data/TEXTURE/explosionSmog.png"
#define PLAYERMISSILESMOG_MAX			(256)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static D3DXMATRIX mtxWorld;
static PLAYERMISSILESMOG smog[PLAYERMISSILESMOG_MAX];
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;

/**************************************
プロトタイプ宣言
***************************************/
void CreateVertexBufferPlayerMissileSmog(void);

/**************************************
初期化処理
***************************************/
void InitPlayerMissileSmog(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		CreateVertexBufferPlayerMissileSmog();

		texture = CreateTextureFromFile((LPSTR)PLAYERMISSILESMOG_TEXTURE, pDevice);
	}

	PLAYERMISSILESMOG *ptr = &smog[0];
	for (int i = 0; i < PLAYERMISSILESMOG_MAX; i++)
	{
		ptr->active = false;
	}
}

/**************************************
終了処理
***************************************/
void UninitPlayerMissileSmog(int num)
{
	PLAYERMISSILESMOG *ptr = &smog[0];
	for (int i = 0; i < PLAYERMISSILESMOG_MAX; i++, ptr++)
	{
		ptr->active = false;
	}

	if (num == 0)
	{
		SAFE_RELEASE(texture);
		SAFE_RELEASE(vtxBuff);
	}
}

/**************************************
更新処理
***************************************/
void UpdatePlayerMissileSmog(void)
{
	PLAYERMISSILESMOG *ptr = &smog[0];
	for (int i = 0; i < PLAYERMISSILESMOG_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		ptr->cntFrame++;
		if (ptr->cntFrame == 60)
		{
			ptr->active = false;
		}
	}
}

/**************************************
描画処理
***************************************/
void DrawPlayerMissileSmog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;

	//rendestate set
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetTexture(0, texture);
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	PLAYERMISSILESMOG *ptr = &smog[0];
	for (int i = 0; i < PLAYERMISSILESMOG_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		//init world
		D3DXMatrixIdentity(&mtxWorld);

		//rotation
		GetInvRotBattleCamera(&mtxWorld);

		//translate
		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		//set
		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);
		pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

		//draw
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	pDevice->SetRenderState(D3DRS_LIGHTING, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

/**************************************
頂点作成処理
***************************************/
void CreateVertexBufferPlayerMissileSmog(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vtxBuff,
		0)))
	{
		return;
	}

	VERTEX_3D *pVtx;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(-PLAYERMISSILESMOG_TEXSIZE, PLAYERMISSILESMOG_TEXSIZE, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(PLAYERMISSILESMOG_TEXSIZE, PLAYERMISSILESMOG_TEXSIZE, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-PLAYERMISSILESMOG_TEXSIZE, -PLAYERMISSILESMOG_TEXSIZE, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(PLAYERMISSILESMOG_TEXSIZE, -PLAYERMISSILESMOG_TEXSIZE, 0.0f);

	pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vtxBuff->Unlock();
}

/**************************************
セット処理
***************************************/
void SetPlayerMissileSmog(D3DXVECTOR3 pos)
{
	PLAYERMISSILESMOG *ptr = &smog[0];
	for (int i = 0; i < PLAYERMISSILESMOG_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = pos;
		ptr->cntFrame = 0;
		ptr->active = true;
		return;
	}
}