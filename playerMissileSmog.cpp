//=====================================
//
//プレイヤーミサイルスモッグ処理[playerMissileSmog.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "particleManager.h"
#include "playerMissileSmog.h"
#include "battleCamera.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERMISSILESMOG_TEXSIZE		(5)
#define PLAYERMISSILESMOG_TEXTURE		"data/TEXTURE/playerMissileSmog.png"
#define PLAYERMISSILESMOG_MAX			(2048)
#define PLAYERMISSILESMOG_TEXDEVIDE_X	(4)
#define PLAYERMISSILESMOG_TEXDEVIDE_Y	(2)
#define PLAYERMISSILESMOG_ANIMMAX		(8)
#define PLAYERMISSILESMOG_ANIMTIME		(5)

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
void SetPlayerMissileUV(void);

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

		//アニメーション
		if (ptr->cntFrame % PLAYERMISSILESMOG_ANIMTIME == 0)
		{
			ptr->patternAnim++;
			if (ptr->patternAnim == PLAYERMISSILESMOG_ANIMMAX)
			{
				ptr->active = false;
			}
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

	//set uv
	SetPlayerMissileUV();

	//rendestate set
	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetTexture(0, texture);
	pDevice->SetFVF(FVF_VERTEX_3D);
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

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
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, NUM_POLYGON);
	}

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
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

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * PLAYERMISSILESMOG_MAX,
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

	for (int i = 0; i < PLAYERMISSILESMOG_MAX; i++, pVtx += 4)
	{
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
	}

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
		ptr->cntFrame = ptr->patternAnim = 0;
		ptr->active = true;
		return;
	}
}

/**************************************
UV設定処理
***************************************/
void SetPlayerMissileUV(void)
{
	VERTEX_3D *pVtx = NULL;
	PLAYERMISSILESMOG *ptr = &smog[0];

	float sizeU = 1.0f / PLAYERMISSILESMOG_TEXDEVIDE_X;
	float sizeV = 1.0f / PLAYERMISSILESMOG_TEXDEVIDE_Y;

	int x, y;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < PLAYERMISSILESMOG_MAX; i++, pVtx += 4, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		x = ptr->patternAnim % PLAYERMISSILESMOG_TEXDEVIDE_X;
		y = ptr->patternAnim / PLAYERMISSILESMOG_TEXDEVIDE_X;

		pVtx[0].tex = D3DXVECTOR2(x * sizeU, y * sizeV);
		pVtx[1].tex = D3DXVECTOR2((x + 1) * sizeU, y * sizeV);
		pVtx[2].tex = D3DXVECTOR2(x * sizeU, (y + 1) * sizeV);
		pVtx[3].tex = D3DXVECTOR2((x + 1) * sizeU, (y + 1) * sizeV);
	}

	vtxBuff->Unlock();
}