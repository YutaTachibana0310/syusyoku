//=====================================
//
//スコア倍率ゲージヘッダ[scoreMagniGauge.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SCOREMAGNIGAUGE_H_
#define _SCOREMAGNIGAUGE_H_

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
void InitScoreMagniGauge(int num);
void UninitScoreMagniGauge(int num);
void UpdateScoreMagniGauge(void);
void DrawScoreMagniGauge(void);
void SetScoreMagniGauge(void);
#endif