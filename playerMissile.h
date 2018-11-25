//=====================================
//
//プレイヤーミサイルヘッダ[playerMissile.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PLAYERMISSILE_H_
#define _PLAYERMISSILE_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERMISSILE_MAX	(64)
#define PLAYERMISSILE_DEFAULT_ANGLE	(D3DXVECTOR3(0.0f, 0.0f, -1.0f))
/**************************************
構造体定義
***************************************/
//プレイヤーミサイルの状態定義
enum PLAYERMISSILE_STATE
{
	PLAYERMISSILE_LAUNCH,
	PLAYERMISSILE_ACCEL,
	PLAYERMISSILE_HOMING,
	PLAYERMISSILE_STRAIGHT,
	PLAYERMISSILE_STATEMAX
};

typedef struct
{
	bool active;
	int state;
	D3DXVECTOR3 pos;
	D3DXQUATERNION rot;
	D3DXVECTOR3 *target;
	float *targetHP;
	bool *targetActive;

	int cntFrame;			//カウントフレーム
	D3DXVECTOR3 velocity;	//進行速度
	float speed;

}PLAYERMISSILE;

/**************************************
プロトタイプ宣言
***************************************/
void InitPlayerMissile(int num);
void UninitPlayerMissile(int num);
void UpdatePlayerMissile(void);
void DrawPlayerMissile(void);
void SetPlayerMissile(D3DXVECTOR3 *target, float *pTargetHP, bool *targetActive, D3DXVECTOR3 pos);
void ColliisonPlayerMissileAndEnemyMissile(void);
void ChangeStatePlayerMissile(PLAYERMISSILE *ptr, int next);

void EnterPlayerMissileLaunch(PLAYERMISSILE *missile);
void EnterPlayerMissileAccel(PLAYERMISSILE *missile);
void EnterPlayerMissileHoming(PLAYERMISSILE *missile);
void EnterPlayerMissileStraight(PLAYERMISSILE *missile);

void UpdatePlayerMissileLaunch(PLAYERMISSILE *missile);
void UpdatePlayerMissileAccel(PLAYERMISSILE *missile);
void UpdatePlayerMissileHoming(PLAYERMISSILE *missile);
void UpdatePlayerMissileStraight(PLAYERMISSILE *missile);

#endif