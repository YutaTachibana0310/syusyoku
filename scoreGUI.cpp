//=============================================================================
//
// プレイヤーバレット処理 [scoreGUI.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "scoreGUI.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SCOREGUI_TEXTURE_NAME	_T("data/TEXTURE/UI/scoreGUI.png")	// プレイヤーバレットのテクスチャ

#define SCOREGUI_TEXTURE_SIZE_X					(60)					// テクスチャサイズX
#define SCOREGUI_TEXTURE_SIZE_Y					(60)					// テクスチャサイズY

#define SCOREGUI_TEXTURE_DIVIDE_X				(10)						// アニメパターンのテクスチャ内分割数（X)
#define SCOREGUI_TEXTURE_DIVIDE_Y				(1)						// アニメパターンのテクスチャ内分割数（Y)

#define SCOREGUI_INITPOS_X						(SCREEN_WIDTH - SCOREGUI_TEXTURE_SIZE_X / 2.0f)		//プレイヤーバレットの初期X座標
#define SCOREGUI_INITPOS_Y						(SCOREGUI_TEXTURE_SIZE_Y * 0.9f)					//プレイヤーバレットの初期Y座標
#define SCOREGUI_OFFSETPOS						(-100.0f)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexScoreGUI(void);				//頂点作成関数
void SetTextureScoreGUI(int cntPattern);	// テクスチャ座標の計算処理
void SetVertexScoreGUI(float offset);					// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク

SCOREGUI scoreGUI;				//プレイヤーバレット配列

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitScoreGUI(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCOREGUI* ptr = &scoreGUI;
	int i;

	ptr->pos = D3DXVECTOR3(SCOREGUI_INITPOS_X, SCOREGUI_INITPOS_Y, 0.0f);
	ptr->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ptr->angle = atan2f(SCOREGUI_TEXTURE_SIZE_Y, SCOREGUI_TEXTURE_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(SCOREGUI_TEXTURE_SIZE_X, SCOREGUI_TEXTURE_SIZE_Y));
	ptr->active = false;

	// 頂点情報の作成
	MakeVertexScoreGUI();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)SCOREGUI_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitScoreGUI(int num)
{
	if (texture != NULL)
	{	// テクスチャの開放
		texture->Release();
		texture = NULL;
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateScoreGUI(void)
{

}

/******************************************************************************
描画処理
******************************************************************************/
void DrawScoreGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCOREGUI *ptr = &scoreGUI;

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//プレイヤーバレットを描画
	int score = 9999995;
	int digitMax = (score == 0) ? 1 : (int)log10f((float)score) + 1;
	int num = 0;
	for (int i = 0; i < digitMax; i++, score /= 10)
	{
		num = score % 10;

		//頂点座標を設定
		SetVertexScoreGUI(SCOREGUI_OFFSETPOS * i);

		// テクスチャ座標を設定
		SetTextureScoreGUI(num);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexScoreGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	// rhwの設定
	vertexWk[0].rhw =
		vertexWk[1].rhw =
		vertexWk[2].rhw =
		vertexWk[3].rhw = 1.0f;

	// 反射光の設定
	vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureScoreGUI(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % SCOREGUI_TEXTURE_DIVIDE_X;
	int y = cntPattern / SCOREGUI_TEXTURE_DIVIDE_X;
	float sizeX = 1.0f / SCOREGUI_TEXTURE_DIVIDE_X;
	float sizeY = 1.0f / SCOREGUI_TEXTURE_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexScoreGUI(float offset)
{
	SCOREGUI *ptr = &scoreGUI;
	// 頂点座標の設定
	vertexWk[0].vtx.x = ptr->pos.x - cosf(ptr->angle + ptr->rot.z) * ptr->radius + offset;
	vertexWk[0].vtx.y = ptr->pos.y - sinf(ptr->angle + ptr->rot.z) * ptr->radius;
	vertexWk[1].vtx.x = ptr->pos.x + cosf(ptr->angle - ptr->rot.z) * ptr->radius + offset;
	vertexWk[1].vtx.y = ptr->pos.y - sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[2].vtx.x = ptr->pos.x - cosf(ptr->angle - ptr->rot.z) * ptr->radius + offset;
	vertexWk[2].vtx.y = ptr->pos.y + sinf(ptr->angle - ptr->rot.z) * ptr->radius;
	vertexWk[3].vtx.x = ptr->pos.x + cosf(ptr->angle + ptr->rot.z) * ptr->radius + offset;
	vertexWk[3].vtx.y = ptr->pos.y + sinf(ptr->angle + ptr->rot.z) * ptr->radius;
}
