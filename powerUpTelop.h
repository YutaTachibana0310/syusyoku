//=====================================
//
//パワーアップテロップヘッダ[powerUpTelop.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _POWERUPTELOP_H_
#define _POWERUPTELOP_H_

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
void InitPowerUpTelop(int num);
void UninitPowerUpTelop(int num);
void UpdatePowerUpTelop(void);
void DrawPowerUpTelop(void);

void StartPowerUpTelopAnimation(int num);

#endif