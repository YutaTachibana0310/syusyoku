//=====================================
//
//パーティクルマネージャヘッダ[particleManager.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define FVF_PARTICLE	(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)

/**************************************
構造体定義
***************************************/
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_PRATICLE;


/**************************************
プロトタイプ宣言
***************************************/
void InitParticleManager(int num);
void UninitParticleManager(void);
void UpdateParticleManager(void);
void DrawParticleManager(void);

#endif