//=====================================
//
//プレイヤーバレットヘッダ[PlayerBulletTrail.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PLAYERBULLETTRAIL_H_
#define _PLAYERBULLETTRAIL_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERBULLETTRAIL_MAX		(2048)
/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;

	D3DXVECTOR3 pos;
	int cntFrame;
}PLAYERBULLET_TRAIL;
/**************************************
プロトタイプ宣言
***************************************/
void InitPlayerBulletTrail(int num);
void UninitPlayerBulletTrail(int num);
void UpdatePlayerBulletTrail(void);
void DrawPlayerBulletTrail(void);
void SetPlayerBulletTrail(D3DXVECTOR3 pos, float alpha);

#endif