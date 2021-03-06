//=====================================
//
//テンプレートヘッダ[playerBullet.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PLAYERBULLET_H_
#define _PLAYERBULLET_H_

#include "main.h"
#include "collider.h"
/**************************************
マクロ定義
***************************************/
#define PLAYERBULLET_MAX				(150)
#define PLAYERBULLET_COLLIDER_RAIDUS	(8.0f)
/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;
	int id;

	D3DXVECTOR3 pos, rot, scale;
	D3DXVECTOR3 moveDir;
	float rotation;
	float speed;
	int cntFrame;

	COLLIDER_SPHERE collider;
	COLLIDER_CUBE collider2;

	bool destroyRequest;
}PLAYERBULLET;
/**************************************
プロトタイプ宣言
***************************************/
void InitPlayerBullet(int num);
void UninitPlayerBullet(int num);
void UpdatePlayerBullet(void);
void DrawPlayerBullet(void);
void SetPlayerBullet(D3DXVECTOR3 pos, float speed);
PLAYERBULLET *GetPlayerBulletAdr(int id);
void SetPlayerBulletState(int state);
void BurstPlayerBullet(PLAYERBULLET *ptr);

#endif