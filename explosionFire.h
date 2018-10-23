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
	D3DXVECTOR3 rot;
	bool active;

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