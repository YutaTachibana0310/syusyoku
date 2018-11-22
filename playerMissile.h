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

/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;
	D3DXVECTOR3 pos, rot;
	D3DXVECTOR3 *target;
	float *targetHP;

	int cntFrame;			//カウントフレーム
	int reachFrame;			//着弾タイミング
	D3DXVECTOR3 velocity;	//進行速度

}PLAYERMISSILE;

/**************************************
プロトタイプ宣言
***************************************/
void InitPlayerMissile(int num);
void UninitPlayerMissile(int num);
void UpdatePlayerMissile(void);
void DrawPlayerMissile(void);
void SetPlayerMissile(D3DXVECTOR3 *target, float *pTargetHP, D3DXVECTOR3 pos);
void ColliisonPlayerMissileAndEnemyMissile(void);
#endif