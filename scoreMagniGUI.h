//=====================================
//
//スコア倍率GUIヘッダ[scoreMagniGUI.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SCOREMAGNIGUI_H_
#define _SCOREMAGNIGUI_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct {
	bool active;
	int cntFrame;
	D3DXVECTOR3 pos;
}SCOREMAGNI_GUI;
/**************************************
プロトタイプ宣言
***************************************/
void InitScoreMagniGUI(int num);
void UninitScoreMagniGUI(int num);
void UpdateScoreMagniGUI(void);
void DrawScoreMagniGUI(LPD3DXEFFECT effect);
void SetScoreMagniGUI(D3DXVECTOR3 pos);
#endif