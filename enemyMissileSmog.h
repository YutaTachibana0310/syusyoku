//=====================================
//
//エネミーミサイルスモッグヘッダ[enemyMissileSmog.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _ENEMYMISSILESMOG_H_
#define _ENEMYMISSILESMOG_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct
{
	D3DXVECTOR3 pos;
	int cntFrame;
	bool active;
}EnemyMissileSmog;
/**************************************
プロトタイプ宣言
***************************************/
void InitEnemyMissileSmog(int num);
void UninitEnemyMissileSmog(void);
void UpdateEnemyMissileSmog(void);
void DrawEnemyMissileSmog(void);
void SetEnemyMissileSmog(D3DXVECTOR3 pos);

#endif