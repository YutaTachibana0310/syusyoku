//=====================================
//
//デバッグシーンヘッダ[debugScene.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _DEBUGSCENE_H_
#define _DEBUGSCENE_H_

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
HRESULT InitDebugScene(int num);
void UninitDebugScene(int num);
void UpdateDebugScene(void);
void DrawDebugScene(void);

#endif