//==================================
//
//エネミーミサイル発射処理[enemyMissileHoming.cpp]
//Author:GP11A341 21 立花雄太
//
//==================================
#include "enemyMissileHoming.h"
#include "playerModel.h"

/***********************************
マクロ定義
***********************************/
#define ENEMYMISSILE_HOMINGANGLE	(0.05f)			//ホーミングの回転角度
#define ENEMYMISSILE_FORWORD		(D3DXVECTOR3(0.0f, 0.0f, -1.0f))	//基準となる進行方向
#define ENEMYMISSILE_HOMINGFRAME	(240)

/***********************************
開始動作
************************************/
void EnemyMissileHomingEnter(ENEMYMISSILE *ptr)
{
	ptr->period = 300;
	ptr->cntFrame = 0;
	ptr->speed = 2.0f;
}

/***********************************
更新処理
************************************/
void EnemyMissileHomingUpdate(ENEMYMISSILE *ptr)
{
	//移動方向のホーミング a = 2(d - vt) / t^2
	//D3DXVECTOR3 diff = GetPlayerAdr(0)->pos - ptr->pos;
	//D3DXVECTOR3 accel = 2.0f * (diff - ptr->velocity * ptr->period) / (ptr->period * ptr->period);
	//ptr->period--;

	//ptr->velocity += accel;
	//ptr->pos += ptr->velocity * ptr->speed;

	D3DXVECTOR3 axis;
	D3DXVECTOR3 targetDir = GetPlayerAdr(0)->pos - ptr->pos;
	D3DXVec3Cross(&axis, &ptr->velocity, &targetDir);
	RotateByQuaternion(&ptr->velocity, &axis, ENEMYMISSILE_HOMINGANGLE , &ptr->velocity);

	//回転クォータニオンを設定
	D3DXVec3Cross(&axis, &ENEMYMISSILE_FORWORD, &ptr->velocity);
	D3DXQuaternionRotationAxis(&ptr->rot, &axis, acosf(D3DXVec3Dot(&ptr->velocity, &ENEMYMISSILE_FORWORD)));

	//座標の更新
	ptr->pos += ptr->velocity * ptr->speed;

	ptr->cntFrame++;

	//遷移
	if (ptr->cntFrame >= ENEMYMISSILE_HOMINGFRAME)
	{
		ChangeStateEnemyMissile(ptr, ENEMYMISSILE_STRAIGHT);
	}
	if (ptr->pos.z < -10.0f)
	{
		ptr->active = false;
	}
}

/***********************************
終了動作
************************************/
void EnemyMissileHomingExit(ENEMYMISSILE *ptr)
{

}
