//=============================================================================
//
// タイトルシーン処理 [titleScene.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "titleScene.h"
#include "input.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define TITLESCENE_TEXTURE_NAME	_T("data/TEXTURE/UI/title.png")	// プレイヤーバレットのテクスチャ

#define TITLESCENE_TEXTURE_SIZE_X (200)			// テクスチャサイズX
#define TITLESCENE_TEXTURE_SIZE_Y (200)			// テクスチャサイズY

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexTitleScene(void);	//頂点作成関数
void SetTextureTitleScene(void);		// テクスチャ座標の計算処理
void SetVertexTitleScene(void);		// 頂点の計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static D3DXVECTOR3 pos;
static float angle, radius;

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitTitleScene(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

	angle = atan2f(TITLESCENE_TEXTURE_SIZE_Y, TITLESCENE_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(TITLESCENE_TEXTURE_SIZE_X, TITLESCENE_TEXTURE_SIZE_Y));

	// 頂点情報の作成
	MakeVertexTitleScene();

	if (num == 0)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)TITLESCENE_TEXTURE_NAME, pDevice);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTitleScene(void)
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
void UpdateTitleScene(void)
{
	if (GetKeyboardTrigger(DIK_Z))
	{
		SetScene(BattleScene);
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawTitleScene(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	// ポリゴンの描画
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
}


/******************************************************************************
頂点の作成
******************************************************************************/
HRESULT MakeVertexTitleScene(void)
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

	SetVertexTitleScene();
	SetTextureTitleScene();

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureTitleScene(void)
{
	// テクスチャ座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexTitleScene(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[0].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[1].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[1].vtx.y = pos.y - sinf(angle) * radius;
	vertexWk[2].vtx.x = pos.x - cosf(angle) * radius;
	vertexWk[2].vtx.y = pos.y + sinf(angle) * radius;
	vertexWk[3].vtx.x = pos.x + cosf(angle) * radius;
	vertexWk[3].vtx.y = pos.y + sinf(angle) * radius;
}
