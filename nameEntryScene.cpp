//=====================================
//
//ネームエントリーシーン処理[nameEntryScene.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "nameEntryScene.h"
#include "sceneManager.h"
#include "nameEntryBG.h"
#include "battleCamera.h"
#include "nameEntryController.h"
#include "bgmManager.h"
#include "particleManager.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
HRESULT InitNameEntryScene(int num)
{
	FadeInBGM(BGM_NAMEENTRY, 30);
	InitNameEntryBG(num);
	InitBattleCamera();
	InitNameEntryController(num);
	return S_OK;
}

/**************************************
終了処理
***************************************/
void UninitNameEntryScene(int num)
{
	UninitNameEntryBG(num);
	UninitNameEntryController(num);
}

/**************************************
更新処理
***************************************/
void UpdateNameEntryScene(void)
{
	UpdateNameEntryBG();
	UpdateNameEntryController();
}

/**************************************
描画処理
***************************************/
void DrawNameEntryScene(void)
{
	SetBattleCamera();
	DrawNameEntryBG();
}
