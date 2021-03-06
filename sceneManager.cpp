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
#include "gameoverScene.h"
#include "stageClearScene.h"
#include "nameEntryScene.h"
#include "GUIManager.h"

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
static int nextScene;

//各シーン初期化処理
static SceneInit Init[] =
{
	InitLogoScene,
	InitTitleScene,
	InitBattleScene,
	InitResultScene,
	InitTutorialScene,
	InitGameoverScene,
	InitStageClearScene,
	InitNameEntryScene
};

//各シーン終了処理
static SceneUninit Uninit[] =
{
	UninitLogoScene,
	UninitTitleScene,
	UninitBattleScene,
	UninitResultScene,
	UninitTutorialScene,
	UninitGameoverScene,
	UninitStageClearScene,
	UninitNameEntryScene
};

//各シーン更新処理
static SceneFunc Update[] =
{
	UpdateLogoScene,
	UpdateTitleScene,
	UpdateBattleScene,
	UpdateResultScene,
	UpdateTutorialScene,
	UpdateGameoverScene,
	UpdateStageClearScene,
	UpdateNameEntryScene
};

//各シーン描画処理
static SceneFunc Draw[] =
{
	DrawLogoScene,
	DrawTitleScene,
	DrawBattleScene,
	DrawResultScene,
	DrawTutorialScene,
	DrawGameoverScene,
	DrawStageClearScene,
	DrawNameEntryScene
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
	nextScene = sceneId;
	UninitScene(1);

	if(nextScene != StageClearScene && nextScene != GamveoverScene)
		UninitParticleManager(1);

	*currentSceneId = sceneId;
	InitGUIManager(1);
	InitScene(1);
}

/**************************************
ネクストシーン取得処理
***************************************/
int GetNextScene(void)
{
	return nextScene;
}