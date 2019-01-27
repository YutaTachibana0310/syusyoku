//=====================================
//
//エネミーバレットヘッダ[EnemyBullet.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _ENEMYBULLET_H_
#define _ENEMYBULLET_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/
#define ENEMYBULLET_MAX		(4096)

/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active = false;

	D3DXVECTOR3 pos;

	D3DXVECTOR3 moveDir;
	float speed;
	COLLIDER_CUBE collider;
}ENEMYBULLET;
/**************************************
プロトタイプ宣言
***************************************/
void InitEnemyBullet(int num);
void UninitEnemyBullet(int num);
void UpdateEnemyBullet(void);
void DrawEnemyBullet(void);
void SetEnemyBullet(D3DXVECTOR3 pos, D3DXVECTOR3 moveDir, float speed);
#endif