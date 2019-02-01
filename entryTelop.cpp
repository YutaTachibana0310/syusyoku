//=====================================
//
//エントリーテロップ処理[entryTelop.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "entryTelop.h"

/**************************************
マクロ定義
***************************************/
#define ENTRYTELOP_TEXTURE_NAME		"data/TEXTURE/UI/entryTelop.png"
#define ENTRYTELOP_FADE_DURATION	(100)
#define ENTRYTELOP_TEX_SIZE_X		(600)
#define ENTRYTELOP_TEX_SIZE_Y		(60)
#define ENTRYTELOP_BASEPOS			(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_HEIGHT - 70.0f, 0.0f))

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static LPDIRECT3DTEXTURE9 texture;
static VERTEX_2D vtxWk[NUM_VERTEX];
static int cntFrame;
static bool isFadeIn;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexEntryTelop(void);
void SetDiffuseEntryTelop(float alpha);

/**************************************
初期化処理
***************************************/
void InitEntryTelop(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		MakeVertexEntryTelop();
		texture = CreateTextureFromFile((LPSTR)ENTRYTELOP_TEXTURE_NAME, pDevice);
		initialized = true;
	}

	cntFrame = 0;
	isFadeIn = true;
}

/**************************************
終了処理
***************************************/
void UninitEntryTelop(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateEntryTelop(void)
{
	int addValue = isFadeIn ? 1 : -1;
	cntFrame += addValue;

	if (cntFrame == 0 || cntFrame == ENTRYTELOP_FADE_DURATION)
	{
		isFadeIn = !isFadeIn;
	}

}

/**************************************
描画処理
***************************************/
void DrawEntryTelop(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetTexture(0, texture);

	SetDiffuseEntryTelop((float)cntFrame / (float)ENTRYTELOP_FADE_DURATION);

	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexEntryTelop(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vtxWk[0].vtx = ENTRYTELOP_BASEPOS + D3DXVECTOR3(-ENTRYTELOP_TEX_SIZE_X, -ENTRYTELOP_TEX_SIZE_Y, 0.0f);
	vtxWk[1].vtx = ENTRYTELOP_BASEPOS + D3DXVECTOR3(ENTRYTELOP_TEX_SIZE_X, -ENTRYTELOP_TEX_SIZE_Y, 0.0f);
	vtxWk[2].vtx = ENTRYTELOP_BASEPOS + D3DXVECTOR3(-ENTRYTELOP_TEX_SIZE_X, ENTRYTELOP_TEX_SIZE_Y, 0.0f);
	vtxWk[3].vtx = ENTRYTELOP_BASEPOS + D3DXVECTOR3(ENTRYTELOP_TEX_SIZE_X, ENTRYTELOP_TEX_SIZE_Y, 0.0f);
}

/**************************************
ディフューズ設定処理
***************************************/
void SetDiffuseEntryTelop(float alpha)
{
	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}

/**************************************
テクスチャ設定処理
***************************************/
void SetTextureEntryTelop(int num)
{
	int y = num % 2;
	float sizeY = 1.0f / 2.0f;

	vtxWk[0].tex = D3DXVECTOR2(0.0, y * sizeY);
	vtxWk[1].tex = D3DXVECTOR2(1.0, y * sizeY);
	vtxWk[2].tex = D3DXVECTOR2(0.0, (y+1) * sizeY);
	vtxWk[3].tex = D3DXVECTOR2(1.0, (y+1) * sizeY);
}