//=====================================
//
//プレイヤーミサイルアクセル処理[playerMissileHoming.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerMissile.h"

/**************************************
マクロ定義
***************************************/

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
void EnterPlayerMissileHoming(PLAYERMISSILE *missile)
{
	missile->cntFrame = 0;
	missile->speed = 20.0f;
}

/**************************************
更新処理
***************************************/
void UpdatePlayerMissileHoming(PLAYERMISSILE *missile)
{
	//速度ベクトルを計算
	D3DXVECTOR3 axis;
	D3DXVECTOR3 diff = *missile->target - missile->pos;
	D3DXVec3Normalize(&missile->velocity, &diff);

	//モデルを回転
	D3DXVec3Cross(&axis, &PLAYERMISSILE_DEFAULT_ANGLE, &missile->velocity);
	D3DXQuaternionRotationAxis(&missile->rot, &axis, acosf(D3DXVec3Dot(&missile->velocity, &PLAYERMISSILE_DEFAULT_ANGLE)));

	//座標更新
	missile->pos += missile->velocity * missile->speed;

	if (!*missile->targetActive)
	{
		ChangeStatePlayerMissile(missile, PLAYERMISSILE_STRAIGHT);
	}

}