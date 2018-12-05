//=====================================
//
//プレイヤーミサイルアクセル処理[playerMissileAccel.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerMissile.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERMISSILE_SPEED_MAX			(60.0f)
#define PLAYERMISSILE_ACCEL_VALUE		(0.5f)
#define PLAYERMISSILE_ACCEL_ROTANGLE	(0.08f)
#define PLAYERMISSILE_ACCEL_END			(30)
#define PLAYERMISSILE_ACCEL_GOALSPEED	(10.0f)

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
void EnterPlayerMissileAccel(PLAYERMISSILE *missile)
{
	missile->cntFrame = 0;
	missile->startSpeed = missile->speed;
}

/**************************************
更新処理
***************************************/
void UpdatePlayerMissileAccel(PLAYERMISSILE *missile)
{
	//速度ベクトルを回転
	D3DXVECTOR3 axis;
	D3DXVECTOR3 diff = *missile->target - missile->pos;
	D3DXVec3Cross(&axis, &missile->velocity, &diff);
	RotateByQuaternion(&missile->velocity, &axis, PLAYERMISSILE_ACCEL_ROTANGLE, &missile->velocity);

	//モデルを回転
	D3DXVec3Cross(&axis, &PLAYERMISSILE_DEFAULT_ANGLE, &missile->velocity);
	D3DXQuaternionRotationAxis(&missile->rot, &axis, acosf(D3DXVec3Dot(&missile->velocity, &PLAYERMISSILE_DEFAULT_ANGLE)));

	//加速
	float t = (float)missile->cntFrame / PLAYERMISSILE_ACCEL_END;
	missile->speed = EaseOutCubic(t, missile->startSpeed, PLAYERMISSILE_ACCEL_GOALSPEED);

	//座標更新
	missile->pos += missile->velocity * missile->speed;

	if (!*missile->targetActive)
	{
		ChangeStatePlayerMissile(missile, PLAYERMISSILE_STRAIGHT);
	}

	missile->cntFrame++;
	if (missile->cntFrame == PLAYERMISSILE_ACCEL_END)
	{
		ChangeStatePlayerMissile(missile, PLAYERMISSILE_HOMING);
	}
}