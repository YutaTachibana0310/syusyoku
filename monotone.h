//=====================================
//
//モノトーンヘッダ[monotone.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _MONOTONE_H_
#define _MONOTONE_H_

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
void InitMonotone(int num);
void UninitMonotone(int num);
void UpdateMonotone(void);
void DrawMonotone(void);
void SetMonotoneEffect(bool state);
#endif