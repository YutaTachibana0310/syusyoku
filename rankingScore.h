//=====================================
//
//ランキングスコアヘッダ[rankingScore.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _RANKINGSCORE_H_
#define _RANKINGSCORE_H_

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
void InitRankingScore(int num);
void UninitRankingScore(int num);
void UpdateRankingScore(void);
void DrawRankingScore(void);

#endif