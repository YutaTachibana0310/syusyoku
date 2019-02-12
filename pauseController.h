//=====================================
//
//ポーズコントローラーヘッダ[pauseController.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PAUSECONTROLLER_H_
#define _PAUSECONTROLLER_H_

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
void InitPauseController(int num);
void UninitPauseController(int num);
void UpdatePauseController(void);
void DrawPauseController(void);
bool GetPauseState(void);

#endif