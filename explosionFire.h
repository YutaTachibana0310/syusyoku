//=====================================
//
//エクスプロージョンファイアヘッダ[explosionFire.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _EXPLOSIONFIRE_H_
#define _EXPLOSIONFIRE_H_

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
	float rot;
	bool active;
	float initSpeed;
	float endSpeed;
	D3DXVECTOR3 moveDir;
	int lifeFrame;
	int cntFrame;
	float scale;
	float alpha;

}EXPLOSIONFIRE;
/**************************************
プロトタイプ宣言
***************************************/
void InitExplosionFire(int num);
void UninitExplosionFire(void);
void UpdateExplosionFire(void);
void DrawExplosionFire(void);
void SetExplosionFire(const D3DXVECTOR3 *pos);

#endif