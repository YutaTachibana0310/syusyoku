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

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitNameEntryController(int num)
{
	scoreIndex = 0;
	nameIndex = 0;
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
	data[scoreIndex].playerName[nameIndex] = WrapAround(0, CHARCHIP_END, data[scoreIndex].playerName[nameIndex] + inputY);
}

/**************************************
描画処理
***************************************/
void DrawNameEntryController(void)
{

}
