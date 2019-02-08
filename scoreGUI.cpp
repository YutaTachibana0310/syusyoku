//=============================================================================
//
// プレイヤーバレット処理 [scoreGUI.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "scoreGUI.h"
#include "dataContainer.h"
#include "debugWindow.h"
#include "numGUI.h"
#include "baseGUI.h"
#include "playerModel.h"
#include "Easing.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define SCOREGUI_TEXTURE_NAME	_T("data/TEXTURE/UI/scoreGUI.png")		// UIのテクスチャ
#define SCOREGUI_TEXTURE_SIZE_X					(240)					// テクスチャサイズX
#define SCOREGUI_TEXTURE_SIZE_Y					(100)					// テクスチャサイズY
#define SCOREGUI_INITPOS						(D3DXVECTOR3(210.0f, 100.0f, 0.0f))

#define SCOREGUI_NUMTEX_SIZE_X					(30.0f)					// 数字テクスチャサイズ
#define SCOREGUI_NUMTEX_SIZE_Y					(40.0f)					// 数字テクスチャサイズ
#define SCOREGUI_NUM_OFFSETPOS					(-30.0f)				// 数字のオフセット座標
#define SCOREGUI_NUM_INITPOS					(D3DXVECTOR3(300.0f, 120.0f, 0.0f))	//数字テクスチャの初期座標

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/

/*****************************************************************************
構造体定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/
static BaseGUI* back;
static float alpha;

/******************************************************************************
初期化処理
******************************************************************************/
HRESULT InitScoreGUI(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		//ベースGUIインスタンス作成
		initialized = true;
		back = new BaseGUI((LPSTR)SCOREGUI_TEXTURE_NAME, SCOREGUI_TEXTURE_SIZE_X, SCOREGUI_TEXTURE_SIZE_Y);
	}

	alpha = 1.0f;
	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitScoreGUI(int num)
{
	if (num == 0)
	{
		//インスタンス解放
		delete back;
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
void DrawScoreGUI(float alpha)
{
	NumGUI* numGui = NumGUI::GetInstance();

	//描画
	back->SetVertex(SCOREGUI_INITPOS);
	back->SetAlpha(alpha);
	back->Draw();

	//スコア数字を描画
	int score = GetCurrentScore();
	int digitMax = (score == 0) ? 1 : (int)log10f((float)score) + 1;
	int num = 0;
	for (int i = 0; i < digitMax; i++, score /= 10)
	{
		num = score % 10;

		//頂点座標を設定
		D3DXVECTOR3 pos = SCOREGUI_NUM_INITPOS + D3DXVECTOR3(SCOREGUI_NUM_OFFSETPOS * i, 0.0f, 0.0f);
		numGui->SetVertex(pos, SCOREGUI_NUMTEX_SIZE_X, SCOREGUI_NUMTEX_SIZE_Y);

		// テクスチャ座標を設定
		numGui->SetTexture(num);

		// ポリゴンの描画
		numGui->Draw(NumScore);
	}
}
