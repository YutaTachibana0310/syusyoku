//=====================================
//
//ゲームオーバーシーンヘッダ[gameoverScene.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _GAMEOVERSCENE_H_
#define _GAMEOVERSCENE_H_

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
HRESULT InitGameoverScene(int num);
void UninitGameoverScene(int num);
void UpdateGameoverScene(void);
void DrawGameoverScene(void);

#endif