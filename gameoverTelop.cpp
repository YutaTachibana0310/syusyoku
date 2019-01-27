//=====================================
//
//ゲームオーバーテロップ処理[gameoverTelop.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "gameoverTelop.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define GAMEOVERTELOP_TEXTURE_NAME		"data/TEXTURE/UI/gameover.png"
#define GAMEOVERTELOP_TEXTURE_SIZE_X	(800.0f)
#define GAMEOVERTELOP_TEXTURE_SIZE_Y	(200.0f)
#define GAMEOVERTELOP_DISP_POS			(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f))
#define GAMEOVERTELOP_FADE_DURATION		(120)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static VERTEX_2D vtxWk[NUM_VERTEX];
static LPDIRECT3DTEXTURE9 texture;
static int cntFrame;

/**************************************
プロトタイプ宣言
***************************************/
void MakevertexGameoverTelop(void);
void SetDiffuseGameoverTelop(float alpha);

/**************************************
初期化処理
***************************************/
void InitGameoverTelop(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		texture = CreateTextureFromFile((LPSTR)GAMEOVERTELOP_TEXTURE_NAME, pDevice);

		MakevertexGameoverTelop();
		initialized = true;
	}
}

/**************************************
終了処理
***************************************/
void UninitGameoverTelop(int num)
{
	if (num == 0)
	{
		SAFE_RELEASE(texture);
	}
}

/**************************************
更新処理
***************************************/
void UpdateGameoverTelop(void)
{
	cntFrame++;
}

/**************************************
描画処理
***************************************/
void DrawGameoverTelop(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pDevice->SetTexture(0, texture);
	pDevice->SetFVF(FVF_VERTEX_2D);

	//透過処理
	float t = (float)cntFrame / (float)GAMEOVERTELOP_FADE_DURATION;
	float alpha = EaseLinear(t, 0.0f, 1.0f);
	SetDiffuseGameoverTelop(alpha);

	//描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vtxWk, sizeof(VERTEX_2D));
}

/**************************************
頂点作成処理
***************************************/
void MakevertexGameoverTelop(void)
{
	vtxWk[0].rhw =
		vtxWk[1].rhw =
		vtxWk[2].rhw =
		vtxWk[3].rhw = 1.0f;

	vtxWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vtxWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vtxWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vtxWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	vtxWk[0].vtx = GAMEOVERTELOP_DISP_POS + D3DXVECTOR3(-GAMEOVERTELOP_TEXTURE_SIZE_X / 2.0f, -GAMEOVERTELOP_TEXTURE_SIZE_Y / 2.0f, 0.0f);
	vtxWk[1].vtx = GAMEOVERTELOP_DISP_POS + D3DXVECTOR3( GAMEOVERTELOP_TEXTURE_SIZE_X / 2.0f, -GAMEOVERTELOP_TEXTURE_SIZE_Y / 2.0f, 0.0f);
	vtxWk[2].vtx = GAMEOVERTELOP_DISP_POS + D3DXVECTOR3(-GAMEOVERTELOP_TEXTURE_SIZE_X / 2.0f,  GAMEOVERTELOP_TEXTURE_SIZE_Y / 2.0f, 0.0f);
	vtxWk[3].vtx = GAMEOVERTELOP_DISP_POS + D3DXVECTOR3( GAMEOVERTELOP_TEXTURE_SIZE_X / 2.0f,  GAMEOVERTELOP_TEXTURE_SIZE_Y / 2.0f, 0.0f);
}

/**************************************
ディフューズ設定処理
***************************************/
void SetDiffuseGameoverTelop(float alpha)
{
	vtxWk[0].diffuse =
		vtxWk[1].diffuse =
		vtxWk[2].diffuse =
		vtxWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}

/**************************************
セット処理
***************************************/
void SetGameoverTelop(void)
{
	cntFrame = 0;
}