//=====================================
//
//スクリーン背景処理[screenBG.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "screenBG.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DVERTEXBUFFER9 vtxBuff;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitScreenBG(int num)
{
	static bool initialized = false;

	if (initialized)
		return;

	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&vtxBuff,
		NULL);

	VERTEX_2D *pVtx = NULL;
	vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].vtx = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].vtx = D3DXVECTOR3((float)SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].vtx = D3DXVECTOR3(0.0f, (float)SCREEN_HEIGHT, 0.0f);
	pVtx[3].vtx = D3DXVECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f);

	pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

	pVtx[0].tex = D3DXVECTOR2(0.0f + 0.5f / (float)SCREEN_WIDTH, 0.0f + 0.5f / (float)SCREEN_HEIGHT);
	pVtx[1].tex = D3DXVECTOR2(1.0f + 0.5f / (float)SCREEN_WIDTH, 0.0f + 0.5f / (float)SCREEN_HEIGHT);
	pVtx[2].tex = D3DXVECTOR2(0.0f + 0.5f / (float)SCREEN_WIDTH, 1.0f + 0.5f / (float)SCREEN_HEIGHT);
	pVtx[3].tex = D3DXVECTOR2(1.0f + 0.5f / (float)SCREEN_WIDTH, 1.0f + 0.5f / (float)SCREEN_HEIGHT);

	pVtx[0].diffuse =
		pVtx[1].diffuse =
		pVtx[2].diffuse =
		pVtx[3].diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	vtxBuff->Unlock();

	initialized = true;
}

/**************************************
終了処理
***************************************/
void UninitScreenBG(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(vtxBuff);
	}
}

/**************************************
更新処理
***************************************/
void UpdateScreenBG(void)
{

}

/**************************************
描画処理
***************************************/
void DrawScreenBG(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);
	pDevice->SetFVF(FVF_VERTEX_2D);
	pDevice->SetStreamSource(0, vtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetTexture(0, NULL);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
}
