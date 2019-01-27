//=============================================================================
//
// ロゴシーン処理 [logoScene.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "camera.h"
#include "logoScene.h"
#include "input.h"
#include "Easing.h"
#include "titleScene.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define LOGOSCENE_TEXTURE_NAME	_T("data/TEXTURE/UI/logo.png")	// プレイヤーバレットのテクスチャ

#define LOGOSCENE_TEXTURE_SIZE_X	(300)			// テクスチャサイズX
#define LOGOSCENE_TEXTURE_SIZE_Y	(150)			// テクスチャサイズY

#define LOGOSCENE_FADEIN_END		(30)
#define LOGOSCENE_FADEWAIT_END		(60)
#define LOGOSCENE_FADEOUT_END		(30)

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexLogoScene(void);			//頂点作成関数
void SetTextureLogoScene(void);				// テクスチャ座標の計算処理
void SetVertexLogoScene(void);				// 頂点の計算処理
void SetLogoTextureAlpha(float alpha);		// アルファ設定処理
/*****************************************************************************
構造体定義
*****************************************************************************/
enum LOGOSCENE_STATE
{
	LOGOSCENE_FADEIN,
	LOGOSCENE_FADEWAIT,
	LOGOSCENE_FADEOUT,
	LOGOSCENE_STATEMAX
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static D3DXVECTOR3 pos;
static float angle, radius;
static int cntFrame;
static LOGOSCENE_STATE state = LOGOSCENE_FADEIN;

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitLogoScene(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	pos = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f);

	angle = atan2f(LOGOSCENE_TEXTURE_SIZE_Y, LOGOSCENE_TEXTURE_SIZE_X);
	radius = D3DXVec2Length(&D3DXVECTOR2(LOGOSCENE_TEXTURE_SIZE_X, LOGOSCENE_TEXTURE_SIZE_Y));

	// 頂点情報の作成
	MakeVertexLogoScene();

	if (!initialized)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)LOGOSCENE_TEXTURE_NAME, pDevice);
		initialized = true;
	}

	//カウントリセット
	cntFrame = 0;

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitLogoScene(int num)
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
void UpdateLogoScene(void)
{
	cntFrame++;
	float t = 0.0f;

	switch (state)
	{
		//フェードイン
	case LOGOSCENE_FADEIN:
		t = (float)cntFrame / LOGOSCENE_FADEIN_END;
		SetLogoTextureAlpha(EaseLinear(t, 0.0f, 1.0f));

		if (cntFrame == LOGOSCENE_FADEIN_END)
		{
			cntFrame = 0;
			state = LOGOSCENE_FADEWAIT;
		}
		break;

		//フェード待機
	case LOGOSCENE_FADEWAIT:
		if (cntFrame == LOGOSCENE_FADEWAIT_END)
		{
			cntFrame = 0;
			InitTitleScene(0);			//読み込みで固まるのでここで一旦ロード
			state = LOGOSCENE_FADEOUT;
		}
		break;

		//フェードアウト
	case LOGOSCENE_FADEOUT:
		t = (float)cntFrame / LOGOSCENE_FADEOUT_END;
		SetLogoTextureAlpha(EaseLinear(t, 1.0f, 0.0f));

		if (cntFrame == LOGOSCENE_FADEOUT_END)
		{
			//cntFrame = 0;
			//state = LOGOSCENE_INPUTWAIT;
			SetScene(TitleScene);
		}
		break;
	}
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawLogoScene(void)
{
	SetCamera();

	
}

/******************************************************************************
ロゴ描画
******************************************************************************/
void DrawCircleLogo(void)
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
HRESULT MakeVertexLogoScene(void)
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

	SetVertexLogoScene();
	SetTextureLogoScene();

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureLogoScene(void)
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
void SetVertexLogoScene(void)
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

/******************************************************************************
アルファ設定処理
******************************************************************************/
void SetLogoTextureAlpha(float alpha)
{
	vertexWk[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	vertexWk[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	vertexWk[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
	vertexWk[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, alpha);
}