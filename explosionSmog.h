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

}EXPLOSIONSMOG;
/**************************************
プロトタイプ宣言
***************************************/
void InitExplosionSmog(int num);
void UninitExplosionSmog(void);
void UpdateExplosionSmog(void);
void DrawExplosionSmog(void);
void SetExplosionSmog(const D3DXVECTOR3 *pos);

#endif