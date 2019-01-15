//=====================================
//
//バトルコントローラヘッダ[battleController.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BATTLECONTROLLER_H_
#define _BATTLECONTROLLER_H_

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
void InitBattleController(int num);
void UninitBattleController(int num);
void UpdateBattleController(void);
void StartBonusTime(void);

#endif