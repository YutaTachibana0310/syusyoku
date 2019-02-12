//=====================================
//
//タイトルシーンGUI処理[TitleSceneGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "titleScene.h"
#include "titleTelop.h"

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
入場処理
***************************************/
void OnEnterTitleSceneGUI(void)
{
	InitTitleTelop(0);
}

/**************************************
更新処理
***************************************/
void OnUpdateTitleSceneGUI(void)
{
	UpdateTitleTelop();
}

/**************************************
描画処理
***************************************/
void OnDrawTitleSceneGUI(void)
{
	DrawTitleLogo();
	DrawTitleTelop();
}