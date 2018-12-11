//=====================================
//
//スモールエネミー逃走処理[smallEnemyEscape.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "smallEnemyModel.h"
#include "EasingVector.h"

/**************************************
マクロ定義
***************************************/
#define SMALLENEMY_ESCAPE_END		(300)
#define SMALLENEMY_ESCAPE_GOALPOS_Z	(3000.0f)
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
void EnterSmallEnemyEscape(SMALLENEMY *enemy)
{
	enemy->cntFrame = 0;
	enemy->startPos = enemy->pos;
	enemy->goalPos = enemy->startPos;
	enemy->goalPos.z = SMALLENEMY_ESCAPE_GOALPOS_Z;
}

/**************************************
更新処理
***************************************/
void UpdateSmallEnemyEscape(SMALLENEMY *enemy)
{
	enemy->cntFrame++;

	float t = (float)enemy->cntFrame / SMALLENEMY_ESCAPE_END;
	enemy->pos = EaseInCubicVector(t, enemy->startPos, enemy->goalPos);

	if (enemy->cntFrame == SMALLENEMY_ESCAPE_END)
	{
		enemy->active = false;
	}
}

/**************************************
退場処理
***************************************/
void ExitSmallEnemyEscape(SMALLENEMY *enemy)
{

}