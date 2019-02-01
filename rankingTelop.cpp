//=====================================
//
//ランキングテロップ処理[rankingTelop.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "rankingTelop.h"

/**************************************
マクロ定義
***************************************/
#define RANKINGTELOP_TEXTURE_NAME		"data/TEXTURE/UI/rankingTelop.png"
#define RANKINGTELOP_SIZE_X				(450.0f)
#define RANKINGTELOP_SIZE_Y				(75.0f)
#define RANKINGTELOP_POS				(D3DXVECTOR3(SCREEN_CENTER_X, 75.0f, 0.0f))

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static VERTEX_2D vtxWk[NUM_VERTEX];

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexRankingTelop(void);

/**************************************
初期化処理
***************************************/
void InitRankingTelop(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		texture = CreateTextureFromFile((LPSTR)RANKINGTELOP_TEXTURE_NAME, pDevice);
		MakeVertexRankingTelop();
	}
}

/**************************************
終了処理
***************************************/
void UninitRankingTelop(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateRankingTelop(void)
{

}

/**************************************
描画処理
***************************************/
void DrawRankingTelop(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture);

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexRankingTelop(void)
{
	vtxWk[0].vtx = RANKINGTELOP_POS + D3DXVECTOR3(-RANKINGTELOP_SIZE_X, -RANKINGTELOP_SIZE_Y, 0.0f);
	vtxWk[1].vtx = RANKINGTELOP_POS + D3DXVECTOR3( RANKINGTELOP_SIZE_X, -RANKINGTELOP_SIZE_Y, 0.0f);
	vtxWk[2].vtx = RANKINGTELOP_POS + D3DXVECTOR3(-RANKINGTELOP_SIZE_X, RANKINGTELOP_SIZE_Y, 0.0f);
	vtxWk[3].vtx = RANKINGTELOP_POS + D3DXVECTOR3(RANKINGTELOP_SIZE_X, RANKINGTELOP_SIZE_Y, 0.0f);

	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}