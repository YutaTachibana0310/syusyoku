//=============================================================================
//
// イージングヘッダ [Easing.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _EASING_H_
#define _EASING_H_

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
float EaseInCubic(float time, float start, float goal, float duration);			//InCubic計算処理
float EaseOutCubic(float time, float start, float goal, float duration);		//OutCubic計算処理
float EaseInOutCubic(float time, float start, float goal, float duration);		//InOutCubic計算処理
float EaseLinear(float time, float start, float goal, float duration);			//Linear計算処理
#endif