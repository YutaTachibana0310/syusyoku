//=====================================
//
//ゲームオーバーテロップヘッダ[gameoverTelop.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _GAMEOVERTELOP_H_
#define _GAMEOVERTELOP_H_

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
void InitGameoverTelop(int num);
void UninitGameoverTelop(int num);
void UpdateGameoverTelop(void);
void DrawGameoverTelop(void);
void SetGameoverTelop(void);

#endif