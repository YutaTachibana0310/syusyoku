//=====================================
//
//データコンテナヘッダ[dataContainer.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _DATACONTAINER_H_
#define _DATACONTAINER_H_

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
void InitDataContainer(int num);
void AddScore(int addValue);
void SetPowerUp(void);

int GetShotLevel(void);
int GetLockonLevel(void);
int GetCurrentScore(void);
int GetHighScore(void);

#endif