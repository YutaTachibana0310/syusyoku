//=====================================
//
//ネームエントリーシーンヘッダ[nameEntryScene.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _NAMEENTRYSCENE_H_
#define _NAMEENTRYSCENE_H_

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
HRESULT InitNameEntryScene(int num);
void UninitNameEntryScene(int num);
void UpdateNameEntryScene(void);
void DrawNameEntryScene(void);

#endif