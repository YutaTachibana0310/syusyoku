//=====================================
//
//ロックオン数GUI処理[lockonNumGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "lockonNumGUI.h"
#include "GUIManager.h"
#include "dataContainer.h"
#include "playerModel.h"
#include "debugWindow.h"
#include "baseGUI.h"
#include "numGUI.h"

/**************************************
マクロ定義
***************************************/
#define LOCKONNUMGUI_TEX_NAME				"data/TEXTURE/UI/lockonGUI.png"
#define LOCKONNUMGUI_TEX_SIZE_X				(200.0f)
#define LOCKONNUMGUI_TEX_SIZE_Y				(100.0f)
#define LOCKONNUMGUI_TEX_INITPOS			(D3DXVECTOR3(150.0f, 550.0f, 0.0f))

#define LOCKONNUMGUI_NUMTEX_SIZE_X			(40.0f)
#define LOCKONNUMGUI_NUMTEX_SIZE_Y			(40.0f)
#define LOCKONNUMGUI_NUMTEX_OFFSET			(-40.0f)
#define LOCKONNUMGUI_NUMTEX_INITPOS			(D3DXVECTOR3(90.0f, 580.0f, 0.0f))

#define LOCKONUMGUI_MAXTEX_INITPOS			(D3DXVECTOR3(240.0f, 580.0f, 0.0f))

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static BaseGUI *back;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitLockonNumGUI(int num)
{
	static bool initialized = false;

	if (initialized)
		return;

	back = new BaseGUI((LPSTR)LOCKONNUMGUI_TEX_NAME, LOCKONNUMGUI_TEX_SIZE_X, LOCKONNUMGUI_TEX_SIZE_Y);
}

/**************************************
終了処理
***************************************/
void UninitLockonNumGUI(int num)
{
	if (num == 0)
	{
		delete back;
	}
}

/**************************************
更新処理
***************************************/
void UpdateLockonNumGUI(void)
{

}

/**************************************
描画処理
***************************************/
void DrawLockonNumGUI(float alpha)
{
	NumGUI *numGui = NumGUI::GetInstance();

	//GUI背景を描画
	back->SetVertex(LOCKONNUMGUI_TEX_INITPOS);
	back->SetAlpha(alpha);
	back->Draw();

	//ロックオン数を描画
	{
		int lockNum = GetPlayerAdr(0)->lockonNum;
		int digitMax = (lockNum == 0) ? 1 : (int)log10f((float)lockNum) + 1;
		for (int i = 0; i < digitMax; i++, lockNum /= 10)
		{
			int num = lockNum % 10;

			D3DXVECTOR3 pos = LOCKONNUMGUI_NUMTEX_INITPOS + D3DXVECTOR3(i * LOCKONNUMGUI_NUMTEX_OFFSET, 0.0f, 0.0f);
			numGui->SetVertex(pos, LOCKONNUMGUI_NUMTEX_SIZE_X, LOCKONNUMGUI_NUMTEX_SIZE_Y);
			numGui->SetTexture(num);
			numGui->Draw(NumScore);
		}
	}

	//最大ロックオン数を描画
	{
		int lockNum = GetLockonMax();
		int digitMax = (lockNum == 0) ? 1 : (int)log10f((float)lockNum) + 1;
		for (int i = 0; i < digitMax; i++, lockNum /= 10)
		{
			int num = lockNum % 10;

			D3DXVECTOR3 pos = LOCKONUMGUI_MAXTEX_INITPOS + D3DXVECTOR3(i * LOCKONNUMGUI_NUMTEX_OFFSET, 0.0f, 0.0f);
			numGui->SetVertex(pos, LOCKONNUMGUI_NUMTEX_SIZE_X, LOCKONNUMGUI_NUMTEX_SIZE_Y);
			numGui->SetTexture(num);
			numGui->Draw(NumScore);
		}
	}
}

