//==============================================
//
//perlinNoise.h[パーリンノイズヘッダ]
//Author:GP11A341 21 立花雄太
//
//==============================================
#ifndef _PERLINNOISE_H_
#define _PERLINNOISE_H_

#include "main.h"

/************************************************
マクロ定義
************************************************/

/***********************************************
構造体定義
************************************************/

/***********************************************
プロトタイプ宣言
*************************************************/
void SettingPerlinNoiseHash(unsigned int seed);
float OctavePerlinNoise(float x, float y);


#endif