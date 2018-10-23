//==================================
//
//エネミーミサイル発射処理[enemyMissileLaunch.cpp]
//Author:GP11A341 21 立花雄太
//
//==================================
#include "enemyMissileLaunch.h"
#include "Easing.h"

#define ENEMYMISSILE_INITSPEED		(5.0f)		//初期スピード
#define ENEMYMISSILE_TARGETSPEED	(20.0f)		//目標スピード
#define ENEMYMISSILE_LAUNCHDURATION	(10)		//発射からホーミングに遷移するフレーム
/***********************************
開始動作
************************************/
void EnemyMissileLaunchEnter(ENEMYMISSILE *ptr)
{
	ptr->active = true;
	ptr->cntFrame = 0;
	ptr->speed = 0.0f;
}

/***********************************
更新処理
************************************/
void EnemyMissileLaunchUpdate(ENEMYMISSILE *ptr)
{
	//速度更新
	ptr->cntFrame++;
	ptr->speed = EaseInOutCubic((float)ptr->cntFrame, ENEMYMISSILE_INITSPEED, ENEMYMISSILE_TARGETSPEED, ENEMYMISSILE_LAUNCHDURATION);
	
	//座標更新
	ptr->pos += ptr->speed * ptr->moveDir;

	//目標スピードに到達していたら遷移
	if (ptr->cntFrame == ENEMYMISSILE_LAUNCHDURATION)
	{
		ChangeStateEnemyMissile(ptr, ENEMYMISSILE_HOMING);
	}
}

/***********************************
終了動作
************************************/
void EnemyMissileLaunchExit(ENEMYMISSILE *ptr)
{

}