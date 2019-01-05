//=====================================
//
//爆発ルミネヘッダ[explosionLumine.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _EXPLOSIONLUMINE_H_
#define _EXPLOSIONLUMINE_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;
	int cntFrame;
	int lifeFrame;
	D3DXVECTOR3 moveDir;
}EXPLOSION_LUMINE;

/**************************************
プロトタイプ宣言
***************************************/
void InitExplosionLumine(int num);
void UninitExplosionLumine(int num);
void UpdateExplosionLumine(void);
void DrawExplosionLumine(LPD3DXEFFECT effect);
void SetExplosionLumine(D3DXVECTOR3 *setPos);

#endif