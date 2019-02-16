//=====================================
//
//ポーズGUIヘッダ[pauseGUI.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PAUSEGUI_H_
#define _PAUSEGUI_H_

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
void InitPauseGUI(int num);
void UninitPauseGUI(int num);
void UpdatePauseGUI(void);
void DrawPauseGUI(int index, bool selected);
void DrawPauseMenuState(int index, bool selected, bool state);

#endif