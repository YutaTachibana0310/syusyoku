//=====================================
//
//シャドウヘッダ[shadow.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

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
void InitShadow(int num);
void UninitShadow(int num);
void UpdateShadow(void);
void DrawShadow(LPD3DXEFFECT effect);
void SetShadow(D3DVECTOR pos);
#endif