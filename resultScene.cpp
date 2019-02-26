//=============================================================================
//
// リザルトシーン処理 [resultScene.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "resultScene.h"
#include "input.h"
#include "cloud.h"
#include "enemyMissile.h"
#include "playerModel.h"
#include "camera.h"
#include "particleManager.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define RESULTSCENE_TEXTURE_NAME	_T("data/TEXTURE/UI/result.png")	// プレイヤーバレットのテクスチャ

#define RESULTSCENE_TEXTURE_SIZE_X (200)			// テクスチャサイズX
#define RESULTSCENE_TEXTURE_SIZE_Y (200)			// テクスチャサイズY

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexResultScene(void);	//頂点作成関数
void SetTextureResultScene(void);		// テクスチャ座標の計算処理
void SetVertexResultScene(void);		// 頂点の計算処理

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
HRESULT InitResultScene(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

	angle = atan2f(RESULTSCENE_TEXTURE_SIZE_Y, RESULTSCENE_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(RESULTSCENE_TEXTURE_SIZE_X, RESULTSCENE_TEXTURE_SIZE_Y));

	// 頂点情報の作成
	MakeVertexResultScene();

	if (!initialized)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)RESULTSCENE_TEXTURE_NAME, pDevice);
		initialized = true;
	}
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitResultScene(int num)
{
	if (num == 0)
	{
		if (texture != NULL)
		{	// テクスチャの開放
			texture->Release();
			texture = NULL;
		}
	}

}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateResultScene(void)
{
	if (GetKeyboardPress(DIK_Z))
	{
		SetScene(TitleScene);
		return;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawResultScene(void)
{
	SetCamera();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	SetCamera();

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
HRESULT MakeVertexResultScene(void)
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

	SetVertexResultScene();
	SetTextureResultScene();

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureResultScene(void)
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
void SetVertexResultScene(void)
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
