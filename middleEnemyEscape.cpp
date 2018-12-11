//=====================================
//
//ミドルエネミー逃走処理[middleEnemyEscape.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "middleEnemyModel.h"
#include "EasingVector.h"

/**************************************
マクロ定義
***************************************/
#define MIDDLEENEMY_ESCAPE_GOALPOS	(D3DXVECTOR3(200.0f, -300.0f, -200.0f))
#define MIDDLEENEMY_ESCAPE_END		(300)

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
void EnterMiddleEnemyEscape(MIDDLEENEMY *enemy)
{
	enemy->cntFrame = 0;

	//移動の始点と終点を設定
	enemy->startPos = enemy->pos;
	enemy->goalPos = D3DXVECTOR3(0.0f, -300.0f, -500.0f);
	if (enemy->pos.x < 0)
	{
		enemy->goalPos *= -1;
	}
}

/**************************************
更新処理
***************************************/
void UpdateMiddleEnemyEscape(MIDDLEENEMY *enemy)
{
	enemy->cntFrame++;

	float t = (float)enemy->cntFrame / MIDDLEENEMY_ESCAPE_END;
	enemy->pos = EaseInCubicVector(t, enemy->startPos, enemy->goalPos);

	if (enemy->cntFrame == MIDDLEENEMY_ESCAPE_END)
	{
		enemy->active = false;
	}
}

/**************************************
退場処理
***************************************/
void ExitMiddleEnemyEscape(MIDDLEENEMY *enemy)
{

}