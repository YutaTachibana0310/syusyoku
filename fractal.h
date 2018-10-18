//===========================================================
//
//フラクタル計算ヘッダ[fractal.h]
//Author：GP11A341 21 立花雄太
//
//===========================================================
#ifndef _FRACTAL_H_
#define _FRACTAL_H_

#include "main.h"

/*
プロトタイプ宣言
*/
void InitializeHeightMap1(float *map, int elemNum, float centerHeight, int center); //2次元のハイトマップ生成
void InitializeHeightMap2(float *map, int elemNum); //3次元のハイトマップ生成
float *getHeightMap2(void);
#endif // !_FRACTAL_H_
