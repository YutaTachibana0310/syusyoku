//=============================================================================
//
// プレイヤーバレット処理 [scoreGUI.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "scoreGUI.h"
#include "dataContainer.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SCOREGUI_TEXTURE_NAME	_T("data/TEXTURE/UI/scoreGUI.png")		// UIのテクスチャ
#define SCOREGUI_NUMTEX_NAME	_T("data/TEXTURE/UI/scoreNum.png")		// 数字のテクスチャ
#define SCOREGUI_TEXTURE_SIZE_X					(480)					// テクスチャサイズX
#define SCOREGUI_TEXTURE_SIZE_Y					(200)					// テクスチャサイズY
#define SCOREGUI_INITPOS						(D3DXVECTOR3(0.0f, -5.0f, 0.0f))

#define SCOREGUI_NUMTEX_SIZE_X					(30.0f)					// 数字テクスチャサイズ
#define SCOREGUI_NUMTEX_SIZE_Y					(40.0f)					// 数字テクスチャサイズ
#define SCOREGUI_NUMTEX_DIVIDE_X				(5)						// アニメパターンのテクスチャ内分割数（X)
#define SCOREGUI_NUMTEX_DIVIDE_Y				(2)						// アニメパターンのテクスチャ内分割数（Y)
#define SCOREGUI_NUM_OFFSETPOS					(-30.0f)				// 数字のオフセット座標
#define SCOREGUI_NUM_INITPOS					(D3DXVECTOR3(350.0f, 110.0f, 0.0f))	//数字テクスチャの初期座標

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
HRESULT MakeVertexScoreGUI(void);				//頂点作成関数
void SetTextureScoreNum(int cntPattern);		// テクスチャ座標の計算処理
void SetVertexScoreGUI(void);					// 頂点の計算処理
void SetVertexScoreNum(float offset);			// 数字テクスチャ頂点計算処理

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static LPDIRECT3DTEXTURE9 texture = NULL;				// テクスチャへのポインタ
static LPDIRECT3DTEXTURE9 numTex = NULL;				// 数字テクスチャ
static VERTEX_2D vertexWk[NUM_VERTEX];					//頂点情報格納ワーク
static SCOREGUI scoreGUI;								//スコアGUI構造体

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitScoreGUI(int num)
{
	static bool initialized = false;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	SCOREGUI* ptr = &scoreGUI;

	ptr->angle = atan2f(SCOREGUI_NUMTEX_SIZE_Y, SCOREGUI_NUMTEX_SIZE_X);
	ptr->radius = D3DXVec2Length(&D3DXVECTOR2(SCOREGUI_NUMTEX_SIZE_X, SCOREGUI_NUMTEX_SIZE_Y));

	// 頂点情報の作成
	MakeVertexScoreGUI();

	if (!initialized)
	{
		// テクスチャの読み込み
		texture = CreateTextureFromFile((LPSTR)SCOREGUI_TEXTURE_NAME, pDevice);
		numTex = CreateTextureFromFile((LPSTR)SCOREGUI_NUMTEX_NAME, pDevice);
		initialized = true;
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitScoreGUI(int num)
{
	if (num == 0)
	{
		// テクスチャの開放
		SAFE_RELEASE(texture);
		SAFE_RELEASE(numTex);
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

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, texture);

	//描画
	SetVertexScoreGUI();
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));

	//スコア数字を描画
	pDevice->SetTexture(0, numTex);
	int score = GetCurrentScore();
	int digitMax = (score == 0) ? 1 : (int)log10f((float)score) + 1;
	int num = 0;
	for (int i = 0; i < digitMax; i++, score /= 10)
	{
		num = score % 10;

		//頂点座標を設定
		SetVertexScoreNum(SCOREGUI_NUM_OFFSETPOS * i);

		// テクスチャ座標を設定
		SetTextureScoreNum(num);

		// ポリゴンの描画
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, vertexWk, sizeof(VERTEX_2D));
	}
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

	//set uv
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	return S_OK;
}

/******************************************************************************
テクスチャ座標の設定
******************************************************************************/
void SetTextureScoreNum(int cntPattern)
{
	// テクスチャ座標の設定
	int x = cntPattern % SCOREGUI_NUMTEX_DIVIDE_X;
	int y = cntPattern / SCOREGUI_NUMTEX_DIVIDE_X;
	float sizeX = 1.0f / SCOREGUI_NUMTEX_DIVIDE_X;
	float sizeY = 1.0f / SCOREGUI_NUMTEX_DIVIDE_Y;

	vertexWk[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
	vertexWk[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
	vertexWk[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
	vertexWk[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
}


/******************************************************************************
頂点座標の設定
******************************************************************************/
void SetVertexScoreGUI(void)
{
	// 頂点座標の設定
	vertexWk[0].vtx = SCOREGUI_INITPOS;
	vertexWk[1].vtx = SCOREGUI_INITPOS + D3DXVECTOR3(SCOREGUI_TEXTURE_SIZE_X, 0.0f, 0.0f);
	vertexWk[2].vtx = SCOREGUI_INITPOS + D3DXVECTOR3(0.0f, SCOREGUI_TEXTURE_SIZE_Y, 0.0f);
	vertexWk[3].vtx = SCOREGUI_INITPOS + D3DXVECTOR3(SCOREGUI_TEXTURE_SIZE_X, SCOREGUI_TEXTURE_SIZE_Y, 0.0f);

	// UV座標の設定
	vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	vertexWk[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	vertexWk[3].tex = D3DXVECTOR2(1.0f, 1.0f);
}

/******************************************************************************
数字テクスチャ頂点座標の設定
******************************************************************************/
void SetVertexScoreNum(float offset)
{
	SCOREGUI *ptr = &scoreGUI;

	D3DXVECTOR3 pos = SCOREGUI_NUM_INITPOS;
	pos.x += offset;

	vertexWk[0].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[0].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[1].vtx.y = pos.y - sinf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.x = pos.x - cosf(ptr->angle) * ptr->radius;
	vertexWk[2].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.x = pos.x + cosf(ptr->angle) * ptr->radius;
	vertexWk[3].vtx.y = pos.y + sinf(ptr->angle) * ptr->radius;
}