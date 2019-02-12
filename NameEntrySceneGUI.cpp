//=====================================
//
//ネームエントリーシーンGUI処理[NameEntrySceneGUI.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "nameEntryRanking.h"
#include "rankingScore.h"
#include "nameEntryCursor.h"
#include "rankingTelop.h"
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

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
入場処理
***************************************/
void OnEnterNameEntrySceneGUI(void)
{
	InitNameEntryRanking(0);
	InitRankingScore(0);
	InitNameEntryCursor(0);
	InitRankingTelop(0);
	InitEntryTelop(0);
}

/**************************************
更新処理
***************************************/
void OnUpdateNameEntrySceneGUI(void)
{
	UpdateNameEntryRanking();
	UpdateRankingScore();
	UpdateNameEntryCursor();
	UpdateRankingTelop();
	UpdateEntryTelop();
}

/**************************************
描画処理
***************************************/
void OnDrawNameEntrySceneGUI(void)
{
	DrawNameEntryRanking();
	DrawRankingScore();
	DrawNameEntryCursor();
	DrawRankingTelop();
	DrawEntryTelop();
}