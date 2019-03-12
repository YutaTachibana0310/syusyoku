//=============================================================================
//
// タイトルシーン処理 [titleScene.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "main.h"
#include "titleScene.h"
#include "input.h"
#include "Easing.h"
#include "meshCylinder.h"
#include "playerModel.h"
#include "playerBullet.h"
#include "playerBulletTrail.h"
#include "battleCamera.h"
#include "sceneFade.h"
#include "bgmManager.h"
#include "soundEffectManager.h"
#include "battleController.h"
#include "baseGUI.h"
#include "titleTelop.h"
#include "particleManager.h"
#include "shockBlur.h"

/*****************************************************************************
マクロ定義
*****************************************************************************/
#define TITLESCENE_FADEIN_END		(60)

#define TITLESCENE_LOGOTEX_NAME		"data/TEXTURE/UI/titlelogo00.png"
#define TITLESCENE_LOGOTEX_SIZE_X	(600)
#define TITLESCENE_LOGOTEX_SIZE_Y	(208)
#define TITLESCENE_LOGOTEX_POS		(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 300.0f, 0.0f))

/*****************************************************************************
プロトタイプ宣言
*****************************************************************************/
void StartGame(void);
void StartGameFromBonus(void);

/*****************************************************************************
構造体定義
*****************************************************************************/
enum TITLESCENE_STATE
{
	TITLESCENE_FADEIN,
	TITLESCENE_INPUTWAIT,
	TITLESCENE_STATEMAX
};

typedef void(*FuncTitleMenu)(void);

enum class GameMode {
	Default,
	BonusPresen,
	Presen
};

/*****************************************************************************
グローバル変数
*****************************************************************************/
static BaseGUI *titleLogo;
static int cntFrame;
static TITLESCENE_STATE state;

//タイトルメニュー処理テーブル
static FuncTitleMenu TitleMenu[2] = {
	StartGame,
	StartGameFromBonus
};

/******************************************s************************************
初期化処理
******************************************************************************/
HRESULT InitTitleScene(int num)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	static bool initialized = false;

	if (!initialized)
	{
		// テクスチャの読み込み
		titleLogo = new BaseGUI((LPSTR)TITLESCENE_LOGOTEX_NAME, TITLESCENE_LOGOTEX_SIZE_X, TITLESCENE_LOGOTEX_SIZE_Y);
		titleLogo->SetVertex(TITLESCENE_LOGOTEX_POS);
		initialized = true;
	}

	InitMeshCylinder(num);
	InitPlayerModel(num);
	InitPlayerBullet(num);
	InitPlayerBulletTrail(num);

	if (num != 0)
	{
		//SetBackColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		PlayBGM(BGM_TITLESCENE);
		titleLogo->SetAlpha(0.0f);

		state = TITLESCENE_FADEIN;
		cntFrame = 0;

		ChangeStatePlayerModel(PlayerTitle);
		SetBattleCameraState(FirstPersonCamera);
	}

	return S_OK;
}

/******************************************************************************
終了処理
******************************************************************************/
void UninitTitleScene(int num)
{
	if (num == 0)
	{
		delete titleLogo;
	}
	else
	{
		UninitMeshCylinder(num);
		UninitPlayerModel(num);
		UninitPlayerBullet(num);
		UninitPlayerBulletTrail(num);
		//UninitParticleManager(num);
	}
}

/******************************************************************************
更新処理
******************************************************************************/
void UpdateTitleScene(void)
{

	if (state == TITLESCENE_FADEIN)
	{
		cntFrame++;
		float t = (float)cntFrame / TITLESCENE_FADEIN_END;

		titleLogo->SetAlpha(EaseLinear(t, 0.0f, 1.0f));
		if (cntFrame == TITLESCENE_FADEIN_END)
		{
			state = TITLESCENE_INPUTWAIT;
		}
	}

	if (GetAttackButtonTrigger() && state == TITLESCENE_INPUTWAIT)
	{
		int selected = GetTitleMenuIndex();
		TitleMenu[selected]();
		SetShockBlur(D3DXVECTOR3(0.0f, 0.0f, 500.0f));
	}

	UpdateMeshCylinder();
	UpdatePlayerModel();
	UpdatePlayerBullet();
	UpdatePlayerBulletTrail();
}

/******************************************************************************
描画処理
******************************************************************************/
void DrawTitleScene(void)
{
	SetBattleCamera();

	DrawMeshCylinder();
	DrawPlayerModel();
	DrawPlayerBullet();
	DrawPlayerBulletTrail();
}

/******************************************************************************
タイトルロゴ描画処理
******************************************************************************/
void DrawTitleLogo(void)
{
	//ロゴ描画
	titleLogo->Draw();
}

/******************************************************************************
ゲームスタート処理
******************************************************************************/
void StartGame(void)
{
	state = TITLESCENE_STATEMAX;
	ChangeStatePlayerModel(PlayerTitleLaunch);

	/*就プレ用に直接バトルシーンへ繊維*/
	//SetSceneFade(TutorialScene);
	SetSceneFade(TutorialScene);

	PlaySE(DefineSE::DECISION);
	FadeOutBGM(BGM_TITLESCENE, TITLESCENE_FADEIN_END);
}

/******************************************************************************
ボーナスタイムからのスタート処理
******************************************************************************/
void StartGameFromBonus(void)
{
	SetBonusTimePresen();

	state = TITLESCENE_STATEMAX;
	ChangeStatePlayerModel(PlayerTitleLaunch);

	SetSceneFade(BattleScene);

	PlaySE(DefineSE::DECISION);
	FadeOutBGM(BGM_TITLESCENE, TITLESCENE_FADEIN_END);
}