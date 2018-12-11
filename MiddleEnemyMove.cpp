//=====================================
//
//ミドルエネミー移動状態処理[middleEnemyMove.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "middleEnemyModel.h"
#include "EasingVector.h"
#include "playerModel.h"
#include "enemyHomingBullet.h"
/**************************************
マクロ定義
***************************************/
#define MIDDLEENEMY_MOVE_END		(180)

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
更新処理
***************************************/
void UpdateMiddleEnemyMove(MIDDLEENEMY *enemy)
{
	enemy->cntFrame++;

	float t = (float)enemy->cntFrame / MIDDLEENEMY_MOVE_END;
	enemy->pos = EaseOutCubicVector(t, enemy->startPos, enemy->goalPos);
	enemy->rot = EaseOutCubicVector(t, enemy->startRot, enemy->goalRot);

	if (enemy->cntFrame == MIDDLEENEMY_MOVE_END)
	{
		ChangeStateMiddleEnemy(enemy, MiddleEnemyAttack);
	}
}

/**************************************
入場処理
***************************************/
void EnterMiddleEnemyMove(MIDDLEENEMY *enemy)
{
	enemy->cntFrame = 0;
	enemy->startPos = enemy->pos;
	enemy->startRot = enemy->rot;
}

/**************************************
退場処理
***************************************/
void ExitMiddleEnemyMove(MIDDLEENEMY *enemy)
{

}

