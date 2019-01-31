//=====================================
//
//ネームエントリーランキング処理[nameEntryRanking.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "nameEntryRanking.h"

/**************************************
マクロ定義
***************************************/
#define NAMEENTRYRANKING_TEXTURE_NAME	"data/TEXTURE/UI/ranking.png"
#define NAMEENTRYRANKING_SIZE_X			(100.0f)
#define NAMEENTRYRANKING_SIZE_Y			(50.0f)
#define NAMEENTRYRANKING_BASEPOS		(D3DXVECTOR3(100.0f, 250.0f, 0.0f))
#define NAMEENTRYRANKING_TEX_X			(1)
#define NAMEENTRYRANKING_TEX_Y			(10)
#define NAMEENTRYRANKING_DIV_X			(2)
#define NAMEENTRYRANKING_DIV_Y			(5)
#define NAMEENTRYRANKING_OFFSET_X		(900.0f)
#define NAMEENTRYRANKING_OFFSET_Y		(150.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static VERTEX_2D vtxWk[NUM_VERTEX];
static LPDIRECT3DTEXTURE9 texture;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexRanking(void);
void SetVertexRanking(int num);
void SetTextureRanking(int num);

/**************************************
初期化処理
***************************************/
void InitNameEntryRanking(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		texture = CreateTextureFromFile((LPSTR)NAMEENTRYRANKING_TEXTURE_NAME, pDevice);
		MakeVertexRanking();
	}
}

/**************************************
終了処理
***************************************/
void UninitNameEntryRanking(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateNameEntryRanking(void)
{

}

/**************************************
描画処理
***************************************/
void DrawNameEntryRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture);
	for (int i = 0; i < NAMEENTRYRANKING_DIV_X * NAMEENTRYRANKING_DIV_Y; i++)
	{
		SetVertexRanking(i);
		SetTextureRanking(i);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
	}
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexRanking(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

/**************************************
頂点設定処理
***************************************/
void SetVertexRanking(int num)
{
	int x = num / NAMEENTRYRANKING_DIV_Y;
	int y = num % NAMEENTRYRANKING_DIV_Y;

	D3DXVECTOR3 basePos = NAMEENTRYRANKING_BASEPOS + D3DXVECTOR3(NAMEENTRYRANKING_OFFSET_X * x, NAMEENTRYRANKING_OFFSET_Y * y, 0.0f);

	vtxWk[0].vtx = basePos + D3DXVECTOR3(-NAMEENTRYRANKING_SIZE_X,-NAMEENTRYRANKING_SIZE_Y, 0.0f);
	vtxWk[1].vtx = basePos + D3DXVECTOR3( NAMEENTRYRANKING_SIZE_X,-NAMEENTRYRANKING_SIZE_Y, 0.0f);
	vtxWk[2].vtx = basePos + D3DXVECTOR3(-NAMEENTRYRANKING_SIZE_X, NAMEENTRYRANKING_SIZE_Y, 0.0f);
	vtxWk[3].vtx = basePos + D3DXVECTOR3( NAMEENTRYRANKING_SIZE_X, NAMEENTRYRANKING_SIZE_Y, 0.0f);
}

/**************************************
UV座標設定処理
***************************************/
void SetTextureRanking(int num)
{
	float sizeY = 1.0f / NAMEENTRYRANKING_TEX_Y;

	vtxWk[0].tex = D3DXVECTOR2(0.0f, sizeY * num);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, sizeY * num);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, (sizeY) * (num + 1));
	vtxWk[3].tex = D3DXVECTOR2(1.0f, (sizeY) * (num + 1));
}