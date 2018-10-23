//==================================
//
//エネミーミサイル発射処理[enemyMissileHoming.cpp]
//Author:GP11A341 21 立花雄太
//
//==================================
#include "enemyMissileHoming.h"

/***********************************
マクロ定義
***********************************/
#define ENEMYMISSILE_HOMINGANGLE	(1.0f)			//ホーミングの回転角度
#define ENEMYMISSILE_FORWORD		(D3DXVECTOR3(0.0f, 0.0f, -1.0f))	//基準となる進行方向

/***********************************
開始動作
************************************/
void EnemyMissileHomingEnter(ENEMYMISSILE *ptr)
{

}

/***********************************
更新処理
************************************/
void EnemyMissileHomingUpdate(ENEMYMISSILE *ptr)
{
	//移動方向のホーミング
	D3DXVECTOR3 axis;
	D3DXVECTOR3 targetDir = ptr->targetPos - ptr->pos;
	D3DXVec3Cross(&axis, &ptr->moveDir, &targetDir);
	RotateByQuaternion(&ptr->moveDir, &axis, ENEMYMISSILE_HOMINGANGLE , &ptr->moveDir);

	//回転クォータニオンを設定
	D3DXVec3Cross(&axis, &ENEMYMISSILE_FORWORD, &ptr->moveDir);
	D3DXQuaternionRotationAxis(&ptr->rot, &axis, acosf(D3DXVec3Dot(&ptr->moveDir, &ENEMYMISSILE_FORWORD)));

	//座標の更新
	ptr->pos += ptr->moveDir * ptr->speed;

	//遷移
	if (GetAngleFromTwoVector(&targetDir, &ptr->moveDir) <= 0.0f)
	{
		ChangeStateEnemyMissile(ptr, ENEMYMISSILE_STRAIGHT);
	}
}

/***********************************
終了動作
************************************/
void EnemyMissileHomingExit(ENEMYMISSILE *ptr)
{

}