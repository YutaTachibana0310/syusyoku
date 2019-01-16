//=====================================
//
//ボーナスタイムGUIヘッダ[bonusTimeGUI.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BONUSTIMEGUI_H_
#define _BONUSTIMEGUI_H_

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
void InitBonusTimeGUI(int num);
void UninitBonusTimeGUI(int num);
void UpdateBonusTimeGUI(void);
void DrawBonusTimeGUI(void);
void StartBonusTimeCount(int count);

#endif