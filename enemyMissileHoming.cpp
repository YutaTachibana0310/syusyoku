//==================================
//
//エネミーミサイル発射処理[enemyMissileHoming.cpp]
//Author:GP11A341 21 立花雄太
//
//==================================
#include "enemyMissileHoming.h"

/***********************************
開始動作
************************************/
void EnemyMissileHomingEnter(ENEMYMISSILE *ptr)
{
	ptr->flgHoming = true;

}

/***********************************
更新処理
************************************/
void EnemyMissileHomingUpdate(ENEMYMISSILE *ptr)
{

}

/***********************************
終了動作
************************************/
void EnemyMissileHomingExit(ENEMYMISSILE *ptr)
{
	ptr->flgHoming = false;
}