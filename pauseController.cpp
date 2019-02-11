//=====================================
//
//テンプレート処理[pauseController.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "pauseController.h"
#include "baseGUI.h"
#include "input.h"

/**************************************
マクロ定義
***************************************/
#define PAUSEMENU_TEXTURE_NAME		"data/TEXTURE/UI/pauseMenu.png"

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static bool flgPause;
static BaseGUI *back;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitPauseController(int num)
{
	static bool initialized = false;

	if (!initialized)
	{
		back = new BaseGUI((LPSTR)PAUSEMENU_TEXTURE_NAME, SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f);
		back->SetVertex(D3DXVECTOR3(SCREEN_CENTER_X, SCREEN_CENTER_Y, 0.0f));
		initialized = true;
	}

	flgPause = false;
	
}

/**************************************
終了処理
***************************************/
void UninitPauseController(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdatePauseController(void)
{
	if (GetKeyboardTrigger(DIK_Q))
	{
		flgPause = !flgPause;
	}
}

/**************************************
描画処理
***************************************/
void DrawPauseController(void)
{
	if (!flgPause)
		return;

	back->Draw();
}

/**************************************
ポーズ状態取得処理
***************************************/
bool GetPauseState(void)
{
	return flgPause;
}