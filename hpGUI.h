//=====================================
//
//HPGUIヘッダ[hpGUI.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _HPGUI_H_
#define _HPGUI_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct {
	float radius, angle;
}HPGUI;
/**************************************
プロトタイプ宣言
***************************************/
void InitHpGUI(int num);
void UninitHpGUI(int num);
void UpdateHpGUI(void);
void DrawHpGUI(float alpha);

#endif