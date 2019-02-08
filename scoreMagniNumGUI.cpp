//=====================================
//
//スコア倍率数字処理[scoreMagniNumGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "scoreMagniNumGUI.h"
#include "baseGUI.h"
#include "dataContainer.h"
#include "GUIManager.h"
#include "numGUI.h"

/**************************************
マクロ定義
***************************************/
#define SCOREMAGNINUM_TEXTURE_NAME		"data/TEXTURE/UI/scoreMagniCross.png"

#define SCOREMAGNINUM_TEXTURE_SIZE_X	(50)
#define SCOREMAGNINUM_TEXTURE_SIZE_Y	(20)
#define SCOREMAGNIGUI_BASEPOS			(D3DXVECTOR3(230.0f, 60.0f, 0.0f))

#define SCOREMAGNIGUI_NUM_BASEPOS		(D3DXVECTOR3(280.0f, 55.0f, 0.0f))
#define SCOREMAGNIGUI_NUM_OFFSET_FLOAT	(-30.0f)
#define SCOREMAGNIGUI_NUM_OFFSET_INT	(-25.0f)
#define SCOREMAGNIGUI_NUM_SIZE_X		(20.0f)
#define SCOREMAGNIGUI_NUM_SIZE_Y		(20.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static BaseGUI *gui;
static int cntFrame;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitScoreMagniNumGUI(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		gui = new BaseGUI((LPSTR)SCOREMAGNINUM_TEXTURE_NAME, SCOREMAGNINUM_TEXTURE_SIZE_X, SCOREMAGNINUM_TEXTURE_SIZE_Y);
		gui->SetVertex(SCOREMAGNIGUI_BASEPOS);
	}
}

/**************************************
終了処理
***************************************/
void UninitScoreMagniNumGUI(int num)
{
	if (num == 0)
	{
		delete gui;
	}
}

/**************************************
更新処理
***************************************/
void UpdateScoreMagniNumGUI(void)
{

}

/**************************************
描画処理
***************************************/
void DrawScoreMagniNumGUI(float alpha)
{
	//GUI背景を描画
	gui->SetAlpha(alpha);
	gui->Draw();

	//GUI数字を描画
	NumGUI *numGui = NumGUI::GetInstance();
	int magni = (int)(GetScoreMagni() * 10);		//小数点第1位まで描画するので10倍して取得
	int digitMax = (magni == 0) ? 1 : (int)log10f((float)magni) + 1;
	for (int i = 0; i < digitMax; i++, magni /= 10)
	{
		int num = magni % 10;
		float offset = (i == 1) ? SCOREMAGNIGUI_NUM_OFFSET_FLOAT : SCOREMAGNIGUI_NUM_OFFSET_INT;
		D3DXVECTOR3 pos = SCOREMAGNIGUI_NUM_BASEPOS + D3DXVECTOR3(offset * i, 0.0f, 0.0f);
		numGui->SetVertex(pos, SCOREMAGNIGUI_NUM_SIZE_X, SCOREMAGNIGUI_NUM_SIZE_Y);
		numGui->SetTexture(num);
		numGui->Draw(NumScore);
	}
}