//=====================================
//
//ボーナステロップヘッダ[bonusTelop.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BONUSTELOP_H_
#define _BONUSTELOP_H_

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
void InitBonusTelop(int num);
void UninitBonusTelop(int num);
void UpdateBonusTelop(void);
void DrawBonusTelop(void);
void StartBonusTelopAnim(bool isStart);

#endif