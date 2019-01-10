//=====================================
//
//キューブパーティクルヘッダ[cubeParticle.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _CUBEOBJECT_H_
#define _CUBEOBJECT_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;
	
	D3DXVECTOR3 moveDir;
	int cntFrame;
	

}CUBE_PARTICLE;

/**************************************
プロトタイプ宣言
***************************************/
void InitCubeParticle(int num);
void UninitCubeParticle(int num);
void UpdateCubeParticle(void);
void DrawCubeParticle(void);

void SetCubeParticle(D3DXVECTOR3 setPos, D3DXCOLOR col);
#endif