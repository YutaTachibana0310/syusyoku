//=====================================
//
//バレットパーティクルヘッダ[bulletParticle.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BULLETPARTICLE_H_
#define _BULLETPARTICLE_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct {
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
}BULLETPARTICLE;

enum BULLETEXPL_TYPE {
	BULLETEXPL_NORMAL,
	BULLETEXPL_HOMING,
	BULLETEXPL_TYPEMAX
};
/**************************************
プロトタイプ宣言
***************************************/
void InitBulletParticle(int num);
void UninitBulletParticle(int num);
void UpdateBulletParticle(void);
void DrawBulletParticle(LPDIRECT3DVERTEXDECLARATION9 declare, LPD3DXEFFECT effect);
void SetBulletParticle(const D3DXVECTOR3 *setPos, BULLETEXPL_TYPE type);
void EmmittBulletParticle(const D3DXVECTOR3 *setPos, BULLETEXPL_TYPE type, int num);
#endif