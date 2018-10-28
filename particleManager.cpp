//=====================================
//
//パーティクルマネージャ処理[particleManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "particleManager.h"
#include "explosionFire.h"
#include "explosionFlare.h"
#include "explosionSmog.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitParticleManager(int num)
{
	InitExplosionSmog(num);
	InitExplosionFire(num);
	InitExplosionFlare(num);
}

/**************************************
終了処理
***************************************/
void UninitParticleManager(void)
{
	UninitExplosionSmog();
	UninitExplosionFire();
	UninitExplosionFlare();
}

/**************************************
更新処理
***************************************/
void UpdateParticleManager(void)
{
	UpdateExplosionSmog();
	UpdateExplosionFire();
	UpdateExplosionFlare();
}

/**************************************
描画処理
***************************************/
void DrawParticleManager(void)
{
	GetDevice()->SetRenderState(D3DRS_LIGHTING, false);

	DrawExplosionSmog();
	DrawExplosionFire();
	DrawExplosionFlare();

	GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
}
