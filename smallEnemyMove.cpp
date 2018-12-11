//=====================================
//
//スモールエネミー移動処理[smallEnemyMove.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "smallEnemyModel.h"
#include "EasingVector.h"

/**************************************
マクロ定義
***************************************/
#define SMALLENEMY_MOVE_END		(120)


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
void EnterSmallEnemyMove(SMALLENEMY *enemy)
{
	enemy->cntFrame = 0;
	enemy->startPos = enemy->pos;
}

/**************************************
更新処理
***************************************/
void UpdateSmallEnemyMove(SMALLENEMY *enemy)
{
	enemy->cntFrame++;

	float t = (float)enemy->cntFrame / SMALLENEMY_MOVE_END;
	enemy->pos = EaseOutCubicVector(t, enemy->startPos, enemy->goalPos);

	if (enemy->cntFrame == SMALLENEMY_MOVE_END)
	{
		ChangeStateSmallEnemy(enemy, SmallEnemyWait);
	}
}

/**************************************
退場処理
***************************************/
void ExitSmallEnemyMove(SMALLENEMY *enemy)
{

}