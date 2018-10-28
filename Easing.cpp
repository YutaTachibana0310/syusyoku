//=============================================================================
//
// イージング処理 [Easing.cpp]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#include "Easing.h"
#include <math.h>

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
マクロ定義
*****************************************************************************/

/*****************************************************************************
グローバル変数
*****************************************************************************/

/*******************************************************************
//関数名	：float EaseInCubic
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInCubic(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = time / duration;

	return diff * t * t + start;
}

/*******************************************************************
//関数名	：float EaseOutCubic
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseOutCubic(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = time / duration;

	return -diff * t * (t - 2.0f) + start;
}

/*******************************************************************
//関数名	：float EaseInOutCubic
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInOutCubic(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = 2.0f * time / duration;

	if (t < 1)
	{
		return diff / 2.0f * t * t + start;
	}
	else
	{
		t -= 1.0f;
		return -diff / 2.0f * (t * (t - 2) - 1) + start;
	}
}

/*******************************************************************
//関数名	：float EaseLinear
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseLinear(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = time / duration;

	return diff * t + start;
}

/*******************************************************************
//関数名	：float EaseInExponential
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInExponential(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = time / duration;

	return diff *  powf(2, 10 * (t - 1)) + start;
}

/*******************************************************************
//関数名	：float EaseOuntExponential
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseOutExponential(float time, float start, float goal, float duration)
{
	float diff = goal - start;
	float t = time / duration;

	return diff * (-powf(2, (-10 * t)) + 1) + start;
}

/*******************************************************************
//関数名	：float EaseInOutExponential
//引数1		：float time		：現在の進行度
//引数2		：float start		：初期値
//引数3		：float goal		：目標値
//引数4		：float duration	：進行度の採集地
//戻り値	：イージングの値
//説明		：イージング計算処理
********************************************************************/
float EaseInOutExponential(float time, float start, float goal, float duration)
{
	return 0.0f;
}