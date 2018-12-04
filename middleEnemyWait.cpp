//=====================================
//
//ミドルエネミー待機処理[middleEnemyWait.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "middleEnemyModel.h"
#include "EasingVector.h"

/**************************************
マクロ定義
***************************************/
#define MIDDLEENEMY_WAIT_END		(120)

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
void EnterMiddleEnemyWait(MIDDLEENEMYMODEL *enemy)
{
	enemy->cntFrame = 0;
}

/**************************************
更新処理
***************************************/
void UpdateMiddleEnemyWait(MIDDLEENEMYMODEL *enemy)
{
	enemy->cntFrame++;
	if (enemy->cntFrame == MIDDLEENEMY_WAIT_END)
	{
		ChangeStateMiddleEnemy(enemy, MiddleEnemyEscape);
	}

}

/**************************************
退場処理
***************************************/
void ExitMiddleEnemyWait(MIDDLEENEMYMODEL *enemy)
{

}