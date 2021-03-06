//=====================================
//
//衝撃ブラーヘッダ[shockBlur.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SHOCKBLUR_H_
#define _SHOCKBLUR_H_

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
void InitShcokBlur(int num);
void UninitShcokBlur(int num);
void UpdateShcokBlur(void);
void DrawShcokBlur(void);

void SetShockBlur(D3DXVECTOR3 setPos, float power = 5.0f);
#endif