//=====================================
//
//テンプレートヘッダ[EnemyManager.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _ENEMYMANAGER_H_
#define _ENEMYMANAGER_H_

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
void InitEnemyManager(int num);
void UninitEnemyManager(int num);
void UpdateEnemyManager(void);
void DrawEnemyManager(void);
void CheckEnemyCollision(void);
#endif