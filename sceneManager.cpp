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

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef void(*SceneFunc)(void);		//シーンの各処理の関数ポインタ定義
typedef HRESULT(*SceneInit)(int num);	//シーンの初期化処理の関数ポインタ定義

/**************************************
グローバル変数
***************************************/
static int currentSceneId = 0;

//各シーン初期化処理
static SceneInit Init[] =
{
	InitLogoScene,
	InitTitleScene,
	InitBattleScene
};

//各シーン終了処理
static SceneFunc Uninit[] =
{
	UninitLogoScene,
	UninitTitleScene,
	UninitBattleScene
};

//各シーン更新処理
static SceneFunc Update[] =
{
	UpdateLogoScene,
	UpdateTitleScene,
	UpdateBattleScene
};

//各シーン描画処理
static SceneFunc Draw[] =
{
	DrawLogoScene,
	DrawTitleScene,
	DrawBattleScene
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitSceneManager(int num)
{
	for (int i = 0; i < DefineSceneMax; i++)
	{
		Init[i](num);
	}
}

/**************************************
個別シーン初期化処理
***************************************/
void InitScene(int num)
{
	Init[currentSceneId](num);
}

/**************************************
終了処理
***************************************/
void UninitSceneManager(void)
{
	for (int i = 0; i < DefineSceneMax; i++)
	{
		Uninit[i]();
	}
}

/**************************************
更新処理
***************************************/
void UpdateSceneManager(void)
{
	Update[currentSceneId]();
}

/**************************************
描画処理
***************************************/
void DrawSceneManager(void)
{
	Draw[currentSceneId]();
}

/**************************************
シーンセット処理
***************************************/
void SetScene(DefineScene sceneId)
{
	currentSceneId = sceneId;
}
