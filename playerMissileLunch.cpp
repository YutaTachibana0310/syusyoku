//=====================================
//
//プレイヤーミサイルランチ処理[playerMissileLaunch.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerMissile.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERMISSILE_LAUNCH_END		(45)

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
void EnterPlayerMissileLaunch(PLAYERMISSILE *missile)
{
	missile->cntFrame = 0;
}

/**************************************
更新処理
***************************************/
void UpdatePlayerMissileLaunch(PLAYERMISSILE *missile)
{
	//速度ベクトルを回転
	D3DXVECTOR3 axis;
	D3DXVECTOR3 diff = *missile->target - missile->pos;
	D3DXVec3Cross(&axis, &missile->velocity, &D3DXVECTOR3(0.0f, 0.0f, 1.0f));
	RotateByQuaternion(&missile->velocity, &axis, 0.1f, &missile->velocity);

	//モデルを回転
	D3DXVec3Cross(&axis, &PLAYERMISSILE_DEFAULT_ANGLE, &missile->velocity);
	D3DXQuaternionRotationAxis(&missile->rot, &axis, acosf(D3DXVec3Dot(&missile->velocity, &PLAYERMISSILE_DEFAULT_ANGLE)));

	//座標更新
	missile->pos += missile->velocity * missile->speed;

	missile->cntFrame++;
	if (missile->cntFrame == PLAYERMISSILE_LAUNCH_END)
	{
		ChangeStatePlayerMissile(missile, PLAYERMISSILE_ACCEL);
	}

}