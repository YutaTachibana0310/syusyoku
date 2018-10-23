//=====================================
//
//エクスプロージョンファイア処理[explosionFire.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "explosionFire.h"
#include "camera.h"

/**************************************
マクロ定義
***************************************/
#define EXPLOSIONFIRE_TEXNAME	"data/TEXTURE/explosionFire.png"
#define EXPLOSIONFIRE_SIZE		(52)
#define EXPLOSIONFIRE_MAX		(128)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;
static LPDIRECT3DVERTEXBUFFER9 vtxBuff = NULL;
static EXPLOSIONFIRE fire[EXPLOSIONFIRE_MAX];
static D3DXMATRIX mtxWorld;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexExplosionFire(void);

/**************************************
初期化処理
***************************************/
void InitExplosionFire(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (num == 0)
	{
		MakeVertexExplosionFire();

		texture = CreateTextureFromFile((LPSTR)EXPLOSIONFIRE_TEXNAME, pDevice);
	}

	EXPLOSIONFIRE *ptr = &fire[0];
	for (int i = 0; i < EXPLOSIONFIRE_MAX; i++, ptr++)
	{
		ptr->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

/**************************************
終了処理
***************************************/
void UninitExplosionFire(void)
{
	SAFE_RELEASE(texture);
	SAFE_RELEASE(vtxBuff);
}

/**************************************
更新処理
***************************************/
void UpdateExplosionFire(void)
{

}

/**************************************
描画処理
***************************************/
void DrawExplosionFire(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxTranslate;

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, texture);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	EXPLOSIONFIRE *ptr = &fire[0];
	for (int i = 0; i < EXPLOSIONFIRE_MAX; i++, ptr++)
	{
		if (!ptr->active)
		{
			continue;
		}

		D3DXMatrixIdentity(&mtxWorld);

		GetInvCameraRotMtx(&mtxWorld);

		D3DXMatrixTranslation(&mtxTranslate, ptr->pos.x, ptr->pos.y, ptr->pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

		pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

		pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	}
	
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexExplosionFire(void)
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

	pVtx[0].vtx = D3DXVECTOR3(-EXPLOSIONFIRE_SIZE, EXPLOSIONFIRE_SIZE, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(EXPLOSIONFIRE_SIZE, EXPLOSIONFIRE_SIZE, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-EXPLOSIONFIRE_SIZE, -EXPLOSIONFIRE_SIZE, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(EXPLOSIONFIRE_SIZE, -EXPLOSIONFIRE_SIZE, 0.0f);

	pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, -1.0f, 1.0f);

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
エクスプロージョンファイアセット処理
***************************************/
void SetExplosionFire(const D3DXVECTOR3 *pos)
{
	EXPLOSIONFIRE *ptr = &fire[0];

	for (int i = 0; i < EXPLOSIONFIRE_MAX; i++, ptr++)
	{
		if (ptr->active)
		{
			continue;
		}

		ptr->pos = *pos;
		ptr->active = true;
		return;
	}
}