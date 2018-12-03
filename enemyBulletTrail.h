//=====================================
//
//エネミーホーミングバレットトレイルヘッダ[EnemyBulletTrail.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _ENEMYBULLETTRAIL_H_
#define _ENEMYBULLETTRAIL_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define ENEMYBULLETTRAIL_MAX		(4096)
/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;

	D3DXVECTOR3 pos;
	int cntFrame;
}ENEMYBULLET_TRAIL;
/**************************************
プロトタイプ宣言
***************************************/
void InitEnemyBulletTrail(int num);
void UninitEnemyBulletTrail(int num);
void UpdateEnemyBulletTrail(void);
void DrawEnemyBulletTrail(void);
void SetEnemyHomingBulletTrail(D3DXVECTOR3 pos);
#endif