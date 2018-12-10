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
#define PLAYERBULLET_MAX				(128)
#define PLAYERBULLET_COLLIDER_RAIDUS	(5.0f)
/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;

	D3DXVECTOR3 pos, rot, scale;
	D3DXVECTOR3 moveDir;
	float rotation;
	float speed;
	int cntFrame;

	SPHERE collider;
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

#endif