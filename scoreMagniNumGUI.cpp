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

/**************************************
マクロ定義
***************************************/
#define SCOREMAGNINUM_TEXTURE_NAME		"data/TEXTURE/UI/scoreMagniNum.png"

#define SCOREMAGNINUM_TEXTURE_SIZE_X	(240)
#define SCOREMAGNINUM_TEXTURE_SIZE_Y	(100)
#define SCOREMAGNIGUI_BASEPOS			(D3DXVECTOR3(SCOREMAGNINUM_TEXTURE_SIZE_X, 300.0f, 0.0f))

#define SCOREMAGNIGUI_NUM_BASEPOS		(D3DXVECTOR3(310.0f, 320.0f, 0.0f))
#define SCOREMAGNIGUI_NUM_OFFSET_FLOAT	(-50.0f)
#define SCOREMAGNIGUI_NUM_OFFSET_INT	(-40.0f)
#define SCOREMAGNIGUI_NUM_SIZE_X		(30.0f)
#define SCOREMAGNIGUI_NUM_SIZE_Y		(30.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static BaseGUI *gui;
static VERTEX_2D vtxWk[NUM_VERTEX];
static int cntFrame;
static LPDIRECT3DTEXTURE9 texture;

/**************************************
プロトタイプ宣言
***************************************/
void MakeVertexScoreMagniNumGUI(void);
void SetVertexScoreMagniNumGUI(float offset);

/**************************************
初期化処理
***************************************/
void InitScoreMagniNumGUI(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		LPDIRECT3DDEVICE9 pDevice = GetDevice();
		MakeVertexScoreMagniNumGUI();

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
		SAFE_RELEASE(texture);
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
void DrawScoreMagniNumGUI(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//GUI背景を描画
	gui->Draw();

	//GUI数字を描画
	int magni = (int)(GetScoreMagni() * 10);		//小数点第1位まで描画するので10倍して取得
	int digitMax = (magni == 0) ? 1 : (int)log10f((float)magni) + 1;
	for (int i = 0; i < digitMax; i++, magni /= 10)
	{
		int num = magni % 10;
		float offset = (i == 1) ? SCOREMAGNIGUI_NUM_OFFSET_FLOAT : SCOREMAGNIGUI_NUM_OFFSET_INT;
		SetVertexScoreMagniNumGUI(i * offset);
		DrawGUINum(GUI_NUMSCORE, num, vtxWk);
	}
}

/**************************************
頂点作成処理
***************************************/
void MakeVertexScoreMagniNumGUI(void)
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
数字頂点設定処理
***************************************/
void SetVertexScoreMagniNumGUI(float offset)
{
	D3DXVECTOR3 basePos = SCOREMAGNIGUI_NUM_BASEPOS;
	basePos.x += offset;

	vtxWk[0].vtx = basePos + D3DXVECTOR3(-SCOREMAGNIGUI_NUM_SIZE_X, -SCOREMAGNIGUI_NUM_SIZE_Y, 0.0f);
	vtxWk[1].vtx = basePos + D3DXVECTOR3( SCOREMAGNIGUI_NUM_SIZE_X, -SCOREMAGNIGUI_NUM_SIZE_Y, 0.0f);
	vtxWk[2].vtx = basePos + D3DXVECTOR3(-SCOREMAGNIGUI_NUM_SIZE_X,  SCOREMAGNIGUI_NUM_SIZE_Y, 0.0f);
	vtxWk[3].vtx = basePos + D3DXVECTOR3( SCOREMAGNIGUI_NUM_SIZE_X,  SCOREMAGNIGUI_NUM_SIZE_Y, 0.0f);
}