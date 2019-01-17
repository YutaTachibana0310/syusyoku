//=====================================
//
//ファジー処理[fuzzy.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "fuzzyLogic.h"
#include <assert.h>

/**************************************
マクロ定義
***************************************/

/**************************************
右上がり斜辺メンバーシップ関数
***************************************/
float fFuzzyRightGrade(float value, float x0, float x1)
{
	assert(x0 <= x1);

	if (value <= x0)
		return 0;

	if (value >= x1)
		return 1;

	float diff = x1 - x0;

	return (value - x0) / diff;
}

/**************************************
左上がり斜辺メンバーシップ関数
***************************************/
float fFuzzyLeftGrade(float value, float x0, float x1)
{
	assert(x0 <= x1);

	if (value <= x0)
		return 1;

	if (value >= x1)
		return 0;

	float diff = x1 - x0;

	return (x1 - value) / diff;
}

/**************************************
三角形メンバーシップ関数
***************************************/
float fFuzzyTriangle(float value, float x0, float x1, float x2)
{
	assert(x0 <= x1);
	assert(x1 <= x2);

	if (value <= x0)
		return 0;

	if (value >= x2)
		return 0;

	if (value > x0 && value < x1)
		return fFuzzyRightGrade(value, x0, x1);

	return fFuzzyLeftGrade(value, x1, x2);
}

/**************************************
台形メンバーシップ関数
***************************************/
float fFuzzyTrapezoid(float value, float x0, float x1, float x2, float x3)
{
	assert(x0 <= x1);
	assert(x1 <= x2);
	assert(x2 <= x3);

	if (value <= x0 || value >= x3)
		return 0;

	if (value >= x1 && value <= x2)
		return 1;

	if (value < x1)
		return fFuzzyRightGrade(value, x0, x1);

	return fFuzzyLeftGrade(value, x2, x3);
}

/**************************************
右上がり斜辺メンバーシップ関数(DWORD)
***************************************/
float dwFuzzyRightGrade(DWORD value, DWORD x0, DWORD x1)
{
	assert(x0 <= x1);

	if (value <= x0)
		return 0;

	if (value >= x1)
		return 1;

	float diff = (float)(x1 - x0);

	return (value - x0) / diff;
}

/**************************************
左上がり斜辺メンバーシップ関数(DWORD)
***************************************/
float dwFuzzyLeftGrade(DWORD value, DWORD x0, DWORD x1)
{
	assert(x0 <= x1);

	if (value <= x0)
		return 1;

	if (value >= x1)
		return 0;

	float diff = (float)(x1 - x0);

	return (x1 - value) / diff;
}

/**************************************
三角形メンバーシップ関数(DWORD)
***************************************/
float dwFuzzyTriangle(DWORD value, DWORD x0, DWORD x1, DWORD x2)
{
	assert(x0 <= x1);
	assert(x1 <= x2);

	if (value <= x0)
		return 0;

	if (value >= x2)
		return 0;

	if (value > x0 && value < x1)
		return dwFuzzyRightGrade(value, x0, x1);

	return dwFuzzyLeftGrade(value, x1, x2);
}

/**************************************
台形メンバーシップ関数(DWORD)
***************************************/
float dwFuzzyTrapezoid(DWORD value, DWORD x0, DWORD x1, DWORD x2, DWORD x3)
{
	assert(x0 <= x1);
	assert(x1 <= x2);
	assert(x2 <= x3);

	if (value <= x0 || value >= x3)
		return 0;

	if (value >= x1 && value <= x2)
		return 1;

	if (value < x1)
		return dwFuzzyRightGrade(value, x0, x1);

	return dwFuzzyLeftGrade(value, x2, x3);
}


/**************************************
ファジーAND関数
***************************************/
float FuzzyAnd(float a, float b)
{
	return min(a, b);
}

/**************************************
ファジーOR関数
***************************************/
float FuzzyOr(float a, float b)
{
	return max(a, b);
}

/**************************************
ファジーNOT関数
***************************************/
float FuzzyNot(float a)
{
	return 1.0f - a;
}