//=====================================
//
//エクスプロージョンスモッグヘッダ[explosionSmog.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _EXPLOSIONSMOG_H_
#define _EXPLOSIONSMOG_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
void InitExplosionSmog(int num);
void UninitExplosionSmog(int num);
void UpdateExplosionSmog(void);
void DrawExplosionSmog(LPDIRECT3DVERTEXDECLARATION9 declare, LPD3DXEFFECT effect);
void SetExplosionSmog(const D3DXVECTOR3 *pos);

#endif