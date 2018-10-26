//=====================================
//
//フレア処理[flare.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "flare.h"
#include "camera.h"

/**************************************
マクロ定義
***************************************/
#define FLARE_TEXNAME		("data/TEXTURE/flare.png")
#define FLARE_INITPOS		(D3DXVECTOR3(400.0f, 300.0f, 1000.0f))
#define FLARE_SIZE			(128)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;
static LPDIRECT3DVERTEXBUFFER9 vtxBuff = NULL;

static D3DXMATRIX mtxWorld;
static D3DXVECTOR3 pos;
/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexFlare(void);

/**************************************
初期化処理
***************************************/
void InitFlare(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pos = GetCameraPos() + FLARE_INITPOS;

	if (num == 0)
	{
		texture = CreateTextureFromFile((LPSTR)FLARE_TEXNAME, pDevice);
		MakeVertexFlare();
	}
}

/**************************************
終了処理
***************************************/
void UninitFlare(void)
{
	SAFE_RELEASE(texture);
	SAFE_RELEASE(vtxBuff);
}

/**************************************
更新処理
***************************************/
void UpdateFlare(void)
{

}

/**************************************
描画処理
***************************************/
void DrawFlare(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranlate;

	D3DXMatrixIdentity(&mtxWorld);

	GetInvCameraRotMtx(&mtxWorld);

	D3DXMatrixTranslation(&mtxTranlate, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranlate);

	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_3D));

	pDevice->SetFVF(FVF_VERTEX_3D);

	pDevice->SetTexture(0, texture);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);

	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

/**************************************
頂点情報作成
***************************************/
void MakeVertexFlare(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&vtxBuff,
		NULL
	)))
	{
		return;
	}

	VERTEX_3D *pVtx;

	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(-FLARE_SIZE, FLARE_SIZE, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3(FLARE_SIZE, FLARE_SIZE, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(-FLARE_SIZE, -FLARE_SIZE, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3(FLARE_SIZE, -FLARE_SIZE, 0.0f);

	pVtx[0].nor =
		pVtx[1].nor =
		pVtx[2].nor =
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, -1.0f);

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