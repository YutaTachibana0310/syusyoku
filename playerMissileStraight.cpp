//=====================================
//
//プレイヤーミサイルストレート処理[playerMissileStraight.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerMissile.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERMISSILE_BORDER_Z		(0.0f)
#define PLAYERMISSILE_STRAIGHT_END	(120)

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
void EnterPlayerMissileStraight(PLAYERMISSILE *missile)
{
	missile->cntFrame = 0;
	missile->speed = 40.0f;
}

/**************************************
更新処理
***************************************/
void UpdatePlayerMissileStraight(PLAYERMISSILE *missile)
{
	//座標更新
	missile->pos += missile->velocity * missile->speed;

	if (missile->pos.z < PLAYERMISSILE_BORDER_Z)
	{
		missile->active = false;
	}

	//カウント更新
	missile->cntFrame++;
	if (missile->cntFrame > PLAYERMISSILE_STRAIGHT_END)
	{
		missile->active = false;
	}
}