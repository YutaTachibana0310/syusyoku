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
void UpdateMiddleEnemyMove(MIDDLEENEMYMODEL *enemy)
{
	enemy->cntFrame++;

	if (enemy->cntFrame > MIDDLEENEMY_MOVE_END)
	{
		if (enemy->cntFrame % 30 == 0)
		{
			D3DXVECTOR3 dir = GetPlayerAdr(0)->pos - enemy->pos;
			D3DXVec3Normalize(&dir, &dir);
			SetEnemyHomingBullet(enemy->pos, dir, 3.0f);
		}
		return;
	}


	float t = (float)enemy->cntFrame / MIDDLEENEMY_MOVE_END;
	enemy->pos = EaseOutCubicVector(t, enemy->startPos, enemy->goalPos);
	enemy->rot = EaseOutCubicVector(t, enemy->startRot, enemy->goalRot);
}

/**************************************
入場処理
***************************************/
void EnterMiddleEnemyMove(MIDDLEENEMYMODEL *enemy)
{
	enemy->cntFrame = 0;
	enemy->startPos = enemy->pos;
	enemy->startRot = enemy->rot;
}

/**************************************
退場処理
***************************************/
void ExitMiddleEnemyMove(MIDDLEENEMYMODEL *enemy)
{

}

