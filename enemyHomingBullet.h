//=====================================
//
//エネミーホーミングバレットヘッダ[EnemyHomingBullet.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _ENEMYHOMINGBULLET_H_
#define _ENEMYHOMINGBULLET_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/
#define ENEMYHOMINGBULLET_MAX		(4096)

/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active = false;

	D3DXVECTOR3 pos;

	int targetPlayerID;
	int cntFrame;
	int reachFrame;
	D3DXVECTOR3 velocity;

	float alpha;
	COLLIDER_CUBE collider;

}ENEMYHOMINGBULLET;
/**************************************
プロトタイプ宣言
***************************************/
void InitEnemyHomingBullet(int num);
void UninitEnemyHomingBullet(int num);
void UpdateEnemyHomingBullet(void);
void DrawEnemyHomingBullet(void);
void SetEnemyHomingBullet(D3DXVECTOR3 pos, D3DXVECTOR3 initVelocity, int reachFrame);
#endif