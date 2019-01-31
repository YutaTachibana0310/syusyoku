//=====================================
//
//ネームエントリーカーソルヘッダ[nameEntryCursor.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _NAMEENTRYCURSOR_H_
#define _NAMEENTRYCURSOR_H_

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
void InitNameEntryCursor(int num);
void UninitNameEntryCursor(int num);
void UpdateNameEntryCursor(void);
void DrawNameEntryCursor(void);
void SetNameEntryCursor(int rank, int offsetIndex);
#endif