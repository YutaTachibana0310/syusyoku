//=====================================
//
//テンプレート処理[pauseController.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "pauseController.h"
#include "baseGUI.h"
#include "input.h"
#include "bgmManager.h"
#include "sceneFade.h"

/**************************************
マクロ定義
***************************************/
#define PAUSEMENU_TEXTURE_NAME		"data/TEXTURE/UI/pauseMenu.png"
#define PAUSEMENU_CURSOE_TEX_NAME	"data/TEXTURE/UI/menuCusor.png"
#define PAUSEMENU_CURSOR_SIZE		(50.0f)
#define PAUSEMENU_MAXNUM			(3)
#define PAUSEMENU_ANIMTIME			(5)
#define PAUSEMENU_ANIMPATTERN		(6)
#define PAUSEMENU_TEX_DIV_X			(3)
#define PAUSEMENU_TEX_DIV_Y			(2)

/**************************************
構造体定義
***************************************/
typedef void(*funcPause)(void);

/**************************************
プロトタイプ宣言
***************************************/
void PauseFuncResume(void);
void PauseFuncRestart(void);
void PauseFuncReturnTitle(void);

/**************************************
グローバル変数
***************************************/
static bool flgPause;
static BaseGUI *back;
static BaseGUI *cursor;

static const D3DXVECTOR3 cursorPos[PAUSEMENU_MAXNUM] = {
	D3DXVECTOR3(500.0f, 370.0f, 0.0f),		//500, 370, 0
	D3DXVECTOR3(630.0f, 500.0f, 0.0f),		//630, 500, 0
	D3DXVECTOR3(820.0f, 640.0f, 0.0f),		//760, 630, 0
};

static int menuIndex;
static int cntFrame;
static int cursorPattern;
static int animIndex;

//ポーズメニュー処理テーブル
static funcPause menuTable[PAUSEMENU_MAXNUM] = {
	PauseFuncResume,
	PauseFuncRestart,
	PauseFuncReturnTitle
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
		cursor = new BaseGUI((LPSTR)PAUSEMENU_CURSOE_TEX_NAME, PAUSEMENU_CURSOR_SIZE, PAUSEMENU_CURSOR_SIZE);
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
	if (num == 0)
	{
		delete back;
		delete cursor;
	}
}

/**************************************
更新処理
***************************************/
void UpdatePauseController(void)
{
	//バトルシーン以外はポーズメニューを開かない
	if (GetCurrentScene() != BattleScene)
		return;

	//ポーズボタン入力判定
	if (GetPauseButtonTrigger())
		flgPause = !flgPause;

	//ポーズ状態でなければ以下は実行しない
	if (!flgPause)
		return;

	//カーソルアニメーション
	cntFrame++;
	if (cntFrame % PAUSEMENU_ANIMTIME == 0)
	{
		animIndex = WrapAround(0, PAUSEMENU_ANIMPATTERN, animIndex + 1);
	}

	//カーソルセット
	cursor->SetTexture(PAUSEMENU_TEX_DIV_X, PAUSEMENU_TEX_DIV_Y, animIndex);
	cursor->SetVertex(cursorPos[menuIndex]);

	//カーソル移動
	int inputY = GetVerticalInputRepeat();
	menuIndex = WrapAround(0, PAUSEMENU_MAXNUM, menuIndex - inputY);
	
	//選択された処理を実行
	if(GetAttackButtonTrigger())
		menuTable[menuIndex]();
}

/**************************************
描画処理
***************************************/
void DrawPauseController(void)
{
	if (!flgPause)
		return;

	back->Draw();
	cursor->Draw();
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