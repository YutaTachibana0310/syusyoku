//=====================================
//
//ポストエフェクトマネージャヘッダ[postEffectManager.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _POSTEFFECTMANAGER_H_
#define _POSTEFFECTMANAGER_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
enum DEFINE_POSTEFFECT {
	EFFECT_SHOCKBLUR,
	EFFECT_MONOTONE,
	EFFECT_MAX,
};
/**************************************
プロトタイプ宣言
***************************************/
void InitPostEffectManager(int num);
void UninitPostEffectManager(int num);
void UpdatePostEffectManager(void);
void DrawPostEffectManager(LPDIRECT3DTEXTURE9 tex[2], LPDIRECT3DSURFACE9 suf[2], LPDIRECT3DSURFACE9 back);

bool CheckActivePostEffect(char effect);
bool SetPostEffectUse(char effect, bool state);
#endif