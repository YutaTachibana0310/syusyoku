//=====================================
//
//パーティクルマネージャヘッダ[particleManager.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PARTICLEMANAGER_H_
#define _PARTICLEMANAGER_H_

#include "main.h"
#include "Easing.h"

/**************************************
マクロ定義
***************************************/
#define PARTICLE_INITPOS		(D3DXVECTOR3(-9999.9f, -9999.9f, -9999.9f))	//初期位置
#define PARTICLE_SCROLLSPEED	(-5.0f)

/**************************************
構造体定義
***************************************/
typedef struct
{
	float x, y, z;
	float u, v;
} VERTEX_PARTICLE;		//パーティクル頂点構造体

typedef struct
{
	float u, v;
} VERTEX_UV;			//パーティクルUV構造体

typedef struct
{
	float r, g, b, a;
} VERTEX_COLOR;			//パーティクルカラー構造体

typedef struct
{
	bool active;
	int cntFrame, lifeFrame;

	//スピード
	float initSpeed, endSpeed;
	EASING_TYPE speedType;

	//スケール
	float initScale, endScale;
	EASING_TYPE scaleType;

	//カラー
	float initRed, initGreen, initBlue, initAlpha;
	float endRed, endGreen, endBlue, endAlpha;
	EASING_TYPE colorType;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 moveDir;	
}PARTICLE;				//パーティクル構造体

/**************************************
プロトタイプ宣言
***************************************/
void InitParticleManager(int num);
void UninitParticleManager(int num);
void UpdateParticleManager(void);
void DrawParticleManager(void);

void SetEnemyExplosion(D3DXVECTOR3 pos);
void SetCubeExplosion(D3DXVECTOR3 pos);
void SetPlayerBulletExplosion(D3DXVECTOR3 pos);
#endif