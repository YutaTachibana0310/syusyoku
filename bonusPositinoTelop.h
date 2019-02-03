//=====================================
//
//ボーナスポジションヘッダ[bonusPositionTelop.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BONUSPOSITIONTELOP_H_
#define _BONUSPOSITIONTELOP_H_

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
void InitBonusPositionTelop(int num);
void UninitBonusPositionTelop(int num);
void UpdateBonusPositionTelop(void);
void DrawBonusPositionTelop(void);
void SetStateBonusPositionTelop(bool state);
void SetPositionBonusPositionTelop(D3DXVECTOR3 setPos);
#endif