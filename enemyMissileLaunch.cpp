//==================================
//
//エネミーミサイル発射処理[enemyMissileLaunch.cpp]
//Author:GP11A341 21 立花雄太
//
//==================================
#include "enemyMissileLaunch.h"

#define ENEMYMISSILE_INITSPEED		(50.0f)		//初期スピード
#define ENEMYMISSILE_TARGETSPEED	(200.0f)	//目標スピード
/***********************************
開始動作
************************************/
void EnemyMissileLaunchEnter(ENEMYMISSILE *ptr)
{
	ptr->active = true;
	ptr->cntFrame = 0;
	ptr->flgHoming = false;

}

/***********************************
更新処理
************************************/
void EnemyMissileLaunchUpdate(ENEMYMISSILE *ptr)
{

}

/***********************************
終了動作
************************************/
void EnemyMissileLaunchExit(ENEMYMISSILE *ptr)
{

}