//=====================================
//
//ポーズコントローラー処理[pauseController.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "pauseController.h"
#include "baseGUI.h"
#include "input.h"
#include "bgmManager.h"
#include "sceneFade.h"
#include "pauseGUI.h"
#include "debugWindow.h"
#include "debugWindow.h"
#include "battleController.h"
#include "collisionManager.h"
#include "soundEffectManager.h"
#include "shockBlur.h"
#include "spikeNoise.h"

/**************************************
マクロ定義
***************************************/
#define PAUSEMENU_TEXTURE_NAME		"data/TEXTURE/UI/pausebg.png"
#define PAUSEMENU_ANIMTIME			(5)
#define PAUSEMENU_ANIMPATTERN		(6)
#define PAUSEMENU_TEX_DIV_X			(3)
#define PAUSEMENU_TEX_DIV_Y			(2)
#define PAUSEMENU_MENUMAX			(10)

/**************************************
構造体定義
***************************************/
typedef void(*funcPause)(void);

enum class DefinePauseMenu {
	Resume,
	Restart,
	ReturnTitle,
	SwitchDebugWindow,
	SwitchUseDivSpace,
	SwitchFPSView,
	SwitchSideView,
	SwitchTopView,
	SetShockBlur,
	SetSpikeNoise
};
/**************************************
プロトタイプ宣言
***************************************/
void PauseFuncResume(void);
void PauseFuncRestart(void);
void PauseFuncReturnTitle(void);
void PuaseFuncSwitchDebugWindow(void);
void PauseFuncSwitchUseDivisionSpace(void);
void PauseFuncSwitchFPSView(void);
void PauseFuncSwitchTopView(void);
void PauseFuncSwitchSideView(void);
void PauseFuncSetShockBlur(void);
void PauseFuncSpikeNoise(void);
bool IsSetStateMenu(int index, bool *pState);

/**************************************
グローバル変数
***************************************/
static bool flgPause;
static BaseGUI *back;

static int menuIndex;
static int cntFrame;

static bool useDivSpace = true;;
static bool drawDebug = false;


//ポーズメニュー処理テーブル
static funcPause menuTable[PAUSEMENU_MENUMAX] = {
	PauseFuncResume,
	PauseFuncRestart,
	PauseFuncReturnTitle,
	PuaseFuncSwitchDebugWindow,
	PauseFuncSwitchUseDivisionSpace,
	PauseFuncSwitchFPSView,
	PauseFuncSwitchSideView,
	PauseFuncSwitchTopView,
	PauseFuncSetShockBlur,
	PauseFuncSpikeNoise

};

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

	InitPauseGUI(num);

	flgPause = false;

}

/**************************************
終了処理
***************************************/
void UninitPauseController(int num)
{
	if (num == 0)
	{
		delete back;

	}

	UninitPauseGUI(num);
}

/**************************************
更新処理
***************************************/
void UpdatePauseController(void)
{
	//バトルシーン以外はポーズメニューを開かない
	//if (GetCurrentScene() != BattleScene)
	//	return;

	//ポーズボタン入力判定
	if (GetPauseButtonTrigger())
	{
		PlaySE(DefineSE::PAUSE);
		flgPause = !flgPause;
	}

	//ポーズ状態でなければ以下は実行しない
	if (!flgPause)
		return;

	//カーソルセット
	int inputY = GetVerticalInputRepeat();
	menuIndex = WrapAround(0, PAUSEMENU_MENUMAX, menuIndex - inputY);

	if (inputY != 0)
		PlaySE(DefineSE::CURSOR);

	//選択された処理を実行
	if (GetAttackButtonTrigger())
	{
		PlaySE(DefineSE::MENUDICISION);
		menuTable[menuIndex]();
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

	for (int i = 0; i < PAUSEMENU_MENUMAX; i++)
	{
		DrawPauseGUI(i, i == menuIndex);

		bool state;
		if (IsSetStateMenu(i, &state))
			DrawPauseMenuState(i, i == menuIndex, state);
	}
}

/**************************************
ポーズ状態取得処理
***************************************/
bool GetPauseState(void)
{
	return flgPause;
}

/**************************************
ポーズ解除処理
***************************************/
void PauseFuncResume(void)
{
	flgPause = false;
}

/**************************************
ゲームリスタート処理
***************************************/
void PauseFuncRestart(void)
{
	SetSceneFade(BattleScene);
	flgPause = false;
}

/**************************************
タイトルリターン処理
***************************************/
void PauseFuncReturnTitle(void)
{
	SetSceneFade(TitleScene);
	flgPause = false;
}

/**************************************
デバッグウィンドウONOFF処理
***************************************/
void PuaseFuncSwitchDebugWindow(void)
{
	drawDebug = !drawDebug;
	SetActiveDebugWindow(drawDebug);
}

/**************************************
空間分割切替処理
***************************************/
void PauseFuncSwitchUseDivisionSpace(void)
{
	useDivSpace = !useDivSpace;
	SetUseDivisionSpace(useDivSpace);
}

/**************************************
FPSビューに切替
***************************************/
void PauseFuncSwitchFPSView(void)
{
	ChangeViewModeBattleController(BattleViewFPS);
	flgPause = false;
}

/**************************************
トップビューに切り替え
***************************************/
void PauseFuncSwitchTopView(void)
{
	ChangeViewModeBattleController(BattleViewTop);
	flgPause = false;
}

/**************************************
サイドビューに切り替え
***************************************/
void PauseFuncSwitchSideView(void)
{
	ChangeViewModeBattleController(BattleViewSide);
	flgPause = false;
}

/**************************************
爆発ブラー表示
***************************************/
void PauseFuncSetShockBlur(void)
{
	SetShockBlur(D3DXVECTOR3(RandomRangef(-200.0f, 200.0f), RandomRangef(-100.0f, 100.0f), 500.0f), 50.0f);
	//flgPause = false;
}

/**************************************
スパイクノイズ表示
***************************************/
void PauseFuncSpikeNoise(void)
{
	SetSpikeNoise();
	//flgPause = false;
}

/**************************************
ステート設定できるメニューかどうか
***************************************/
bool IsSetStateMenu(int index, bool *pState)
{
	if (index == static_cast<int>(DefinePauseMenu::SwitchDebugWindow))
	{
		*pState = drawDebug;
		return true;
	}

	if (index == static_cast<int>(DefinePauseMenu::SwitchUseDivSpace))
	{
		*pState = useDivSpace;
		return true;
	}

	return false;
}