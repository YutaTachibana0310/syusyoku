//==================================================
//
//perlinNoise.cpp[パーリンノイズ処理]
//Author:GP11A341 21 立花雄太
//
//==================================================

#include "perlinNoise.h"

/************************************************
マクロ定義
************************************************/
#define PERLINNOISE_HASH_MAX		(256)
#define PARLINNOISE_HASHTABLE_NUM	(PERLINNOISE_HASH_MAX * 2)

/***********************************************
構造体定義
************************************************/

/***********************************************
プロトタイプ宣言
*************************************************/
void SettingPerlnNoiseHash(unsigned int seed);
unsigned int GetHash(int x, int y);
float GetValue(int x, int y);
float Fade(float t);
float Lerp(float a, float b, float t);
float Grad(unsigned int hash, float a, float b);
float PerlinNoise(float x, float y);

/*************************************************
グローバル変数
**************************************************/
static int hashCode[PARLINNOISE_HASHTABLE_NUM] = {};

/**********************************************
乱数テーブル作成
**********************************************/
void SettingPerlinNoiseHash(unsigned int seed)
{
	//乱数初期化
	srand(seed);

	//ハッシュコード初期化
	memset(hashCode, 0, sizeof(unsigned int) * _countof(hashCode));

	//ランダムテーブル作成
	const int TABLE_NUM = PERLINNOISE_HASH_MAX;
	unsigned int randomTable[TABLE_NUM] = {};
	for (int i = 0; i < TABLE_NUM; i++)
	{
		randomTable[i] = rand() % PERLINNOISE_HASH_MAX;
	}

	//ハッシュコード生成
	for (int i = 0; i < PERLINNOISE_HASH_MAX; i++)
	{
		hashCode[i] = randomTable[i % TABLE_NUM];
	}
}

/**********************************************
乱数値取得
**********************************************/
unsigned int GetHash(int x, int y)
{
	x %= PERLINNOISE_HASH_MAX;
	y %= PERLINNOISE_HASH_MAX;
	return hashCode[x + hashCode[y]];
}

/********************************************
乱数を0.0f～1.0fに正規化したものを取得する
*********************************************/
float GetValue(int x, int y)
{
	return (float)GetHash(x, y) / (float)(PERLINNOISE_HASH_MAX - 1);
}

/*******************************************
五次補間関数
*******************************************/
float Fade(float t)
{
	//6x^5 - 15x^4 + 10x^3
	return (6 * powf(t, 5) - 15 * powf(t, 4) + 10 * powf(t, 3));
}

/******************************************
線形補間
******************************************/
float Lerp(float a, float b, float t)
{
	return (a + (b - a) * t);
}

/******************************************
勾配決定関数
*******************************************/
float Grad(unsigned int hash, float a, float b)
{
	unsigned int key = hash % 0x4;

	switch (key)
	{
	case 0x0:
		return a;
	case 0x1:
		return -a;
	case 0x2:
		return -b;
	case 0x3:
		return b;
	}
	return 0.0f;
}

/*****************************************
パーリンノイズ取得関数
******************************************/
float PerlinNoise(float x, float y)
{
	//整数部分と小数部分に分割
	int xi = (int)floorf(x);
	int yi = (int)floorf(y);
	float xf = x - xi;
	float yf = y - yi;

	//格子点からハッシュを取り出し、その値をもとに勾配を取得
	float a00 = Grad(GetHash(xi, yi), xf, yf);
	float a10 = Grad(GetHash(xi + 1, yi), xf - 1.0f, yf);
	float a01 = Grad(GetHash(xi, yi + 1), xf, yf - 1.0f);
	float a11 = Grad(GetHash(xi + 1, yi + 1), xf - 1.0f, yf - 1.0f);

	//補間をかける
	xf = Fade(xf);
	yf = Fade(yf);

	float result = (Lerp(Lerp(a00, a10, xf), Lerp(a01, a11, xf), yf), +1.0f) / 2.0f;
	return result;
}

/****************************************
オクターブパーリンノイズ取得関数
*****************************************/
float OctavePerlinNoise(float x, float y)
{
	float a = 1.0f;
	float f = 1.0f;
	float maxValue = 0.0f;
	float toralValue = 0.0f;
	float per = 0.5f;
	for (int i = 0; i < 5; i++)
	{
		toralValue += a * PerlinNoise(x * f, y * f);
		a *= per;
		f *= 2.0f;
	}
	return toralValue / maxValue;
}