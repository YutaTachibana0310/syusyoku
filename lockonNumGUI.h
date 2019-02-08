//=====================================
//
//ロックオン数GUIヘッダ[lockonNumGUI.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _LOCKONNUMGUI_H_
#define _LOCKONNUMGUI_H_

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
void InitLockonNumGUI(int num);
void UninitLockonNumGUI(int num);
void UpdateLockonNumGUI(void);
void DrawLockonNumGUI(float alpha);
#endif