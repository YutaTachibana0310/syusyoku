//=====================================
//
//ネームエントリーコントローラ処理[nameEntryController.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "nameEntryController.h"
#include "dataContainer.h"
#include "rankingName.h"
#include "input.h"
#include "nameEntryCursor.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static unsigned int scoreIndex;
static unsigned int nameIndex;
static DATA_HIGHSCRE *highScore = NULL;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitNameEntryController(int num)
{
	scoreIndex = 1;
	nameIndex = 0;

	highScore = GetHighScore(scoreIndex);
	for (int i = 0; i < DATACONTAINER_PLAYERNAME_MAX; i++)
	{
		highScore->playerName[i] = 31;
	}
}

/**************************************
終了処理
***************************************/
void UninitNameEntryController(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdateNameEntryController(void)
{
	DATA_HIGHSCRE *data = GetHighScore();
	int inputX = GetHorizontalInputRepeat();
	int inputY = GetVerticalInputRepeat();

	nameIndex = Clamp(0, DATACONTAINER_PLAYERNAME_MAX - 1, nameIndex + inputX);

	if(highScore->playerName[nameIndex] == 31 )
		
	if(highScore->playerName[nameIndex] != 31)
		highScore->playerName[nameIndex] = WrapAround(0, CHARCHIP_END, highScore->playerName[nameIndex] - inputY);

	SetNameEntryCursor(scoreIndex, nameIndex);
}

/**************************************
描画処理
***************************************/
void DrawNameEntryController(void)
{

}
