//=====================================
//
//エントリーテロップヘッダ[entryTelop.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _ENTRYTELOP_H_
#define _ENTRYTELOP_H_

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
void InitEntryTelop(int num);
void UninitEntryTelop(int num);
void UpdateEntryTelop(void);
void DrawEntryTelop(void);
void SetTextureEntryTelop(int num);
#endif