//=====================================
//
//スパイクノイズヘッダ[spikeNoise.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SPIKENOISE_H_
#define _SPIKENOISE_H_

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
void InitSpikeNoise(int num);
void UninitSpikeNoise(int num);
void UpdateSpikeNoise(void);
void DrawSpikeNoise(void);
void SetSpikeNoise(void);
#endif