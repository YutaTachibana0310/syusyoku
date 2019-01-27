//=====================================
//
//ステージクリアテロップヘッダ[stageClearTelop.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _STAGECLEARTELOP_H_
#define _STAGECLEARTELOP_H_

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
void InitStageClearTelop(int num);
void UninitStageClearTelop(int num);
void UpdateStageClearTelop(void);
void DrawStageClearTelop(void);
void SetStageClearTelop(void);

#endif