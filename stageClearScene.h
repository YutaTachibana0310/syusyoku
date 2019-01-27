//=====================================
//
//ステージクリアヘッダ[stageClearScene.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _STAGECLEARSCENE_H_
#define _STAGECLEARSCENE_H_

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
HRESULT InitStageClearScene(int num);
void UninitStageClearScene(int num);
void UpdateStageClearScene(void);
void DrawStageClearScene(void);

#endif