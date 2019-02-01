//=====================================
//
//テンプレート処理[nameEntryInput.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "nameEntryController.h"
#include "input.h"
#include "sceneManager.h"
#include "sceneFade.h"
#include "bgmManager.h"
#include "entryTelop.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static bool flgWait;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
入場処理
***************************************/
void OnEnterNameEntryEnd(NAMEENTRY *entity)
{
	SaveHighScoreData();
	flgWait = false;

	SetTextureEntryTelop(1);
}

/**************************************
更新処理
***************************************/
void OnUpdateNameEntryEnd(NAMEENTRY *entity)
{
	if (flgWait)
		return;

	if (GetAttackButtonTrigger())
	{
		flgWait = true;
		FadeOutBGM(BGM_NAMEENTRY, 30);
		SetSceneFade(TitleScene);
		return;
	}

	if (GetHorizontalInputRepeat() == 1)
	{
		flgWait = true;
		FadeOutBGM(BGM_NAMEENTRY, 30);
		SetSceneFade(TitleScene);
		return;
	}
}