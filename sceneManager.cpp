//=====================================
//
//シーンマネージャ処理[sceneManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "sceneManager.h"
#include "logoScene.h"
#include "titleScene.h"
#include "battleScene.h"
#include "resultScene.h"
#include "particleManager.h"
#include "tutorialScene.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef void(*SceneFunc)(void);			//シーンの各処理の関数ポインタ定義
typedef HRESULT(*SceneInit)(int num);	//シーンの初期化処理の関数ポインタ定義
typedef void(*SceneUninit)(int num);	//シーンの終了処理の関数ポインタ定義

/**************************************
グローバル変数
***************************************/
static int* currentSceneId;

//各シーン初期化処理
static SceneInit Init[] =
{
	InitLogoScene,
	InitTitleScene,
	InitBattleScene,
	InitResultScene,
	InitTutorialScene
};

//各シーン終了処理
static SceneUninit Uninit[] =
{
	UninitLogoScene,
	UninitTitleScene,
	UninitBattleScene,
	UninitResultScene,
	UninitTutorialScene
};

//各シーン更新処理
static SceneFunc Update[] =
{
	UpdateLogoScene,
	UpdateTitleScene,
	UpdateBattleScene,
	UpdateResultScene,
	UpdateTutorialScene
};

//各シーン描画処理
static SceneFunc Draw[] =
{
	DrawLogoScene,
	DrawTitleScene,
	DrawBattleScene,
	DrawResultScene,
	DrawTutorialScene
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitSceneManager(int* ptr)
{

	/*for (int i = 0; i < DefineSceneMax; i++)
	{
		Init[i](num);
	}*/

	currentSceneId = ptr;
}

/**************************************
個別シーン初期化処理
***************************************/
void InitScene(int num)
{
	Init[*currentSceneId](num);
}

/**************************************
終了処理
***************************************/
void UninitSceneManager(int num)
{
	for (int i = 0; i < DefineSceneMax; i++)
	{
		Uninit[i](num);
	}
}

/**************************************
個別シーン終了処理
***************************************/
void UninitScene(int num)
{
	Uninit[*currentSceneId](num);
}

/**************************************
更新処理
***************************************/
void UpdateSceneManager(void)
{
	Update[*currentSceneId]();
}

/**************************************
描画処理
***************************************/
void DrawSceneManager(void)
{
	Draw[*currentSceneId]();
}

/**************************************
シーンセット処理
***************************************/
void SetScene(DefineScene sceneId)
{
	UninitScene(1);
	UninitParticleManager(1);
	*currentSceneId = sceneId;
	InitScene(1);
}
