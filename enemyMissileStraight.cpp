//==================================
//
//エネミーミサイル発射処理[enemyMissileStraight.cpp]
//Author:GP11A341 21 立花雄太
//
//==================================
#include "enemyMissileStraight.h"

/***********************************
マクロ定義
************************************/
#define ENEMYMISSILE_BORDER_Z		(-200.0f)		//ミサイルのZ座標限界値

/***********************************
開始動作
************************************/
void EnemyMissileStraightEnter(ENEMYMISSILE *ptr)
{
	
}

/***********************************
更新処理
************************************/
void EnemyMissileStraightUpdate(ENEMYMISSILE *ptr)
{
	//座標の更新
	ptr->pos += ptr->speed * ptr->moveDir;

	//非アクティブ化
	if (ptr->pos.z < ENEMYMISSILE_BORDER_Z)
	{
		ptr->active = false;
	}
}

/***********************************
終了動作
************************************/
void EnemyMissileStraightExit(ENEMYMISSILE *ptr)
{

}