//=====================================
//
//チュートリアルシーンヘッダ[tutorialScene.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _TUTORIALSCENE_H_
#define _TUTORIALSCENE_H_

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
HRESULT InitTutorialScene(int num);
void UninitTutorialScene(int num);
void UpdateTutorialScene(void);
void DrawTutorialScene(void);

#endif