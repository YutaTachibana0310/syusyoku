//===========================================================
//
//フラクタル計算処理[fractal.cpp]
//Author：GP11A341 21 立花雄太
//
//===========================================================

#include "fractal.h"
#include "MyLibrary.h"
#include "input.h"

/******************************************************
構造体定義
******************************************************/
typedef struct
{
	int z;
	int x;
	float height;
}Point;

/****************************************************
プロトタイプ宣言
****************************************************/
void CreateHeightMap1(int start, int end, float range, float *map);
void CreateHeightMap2(float *map);

/***************************************************
グローバル変数
***************************************************/
static float d = 2.0f;
static const float minHeight = -400.0f, maxHeight = 400.0f;
static int vtxNum;

/**************************************
フラクタルセットアップ関数
***************************************/
void SetUpFractal(int num, float seed)
{
	vtxNum = num;
	d = seed;
}

/*************************************************
関数	：void InitializeHeightMap1(void)
引数	：float *heightmap	：生成したハイトマップを保存するポインタ
引数	：int elemNum		：ハイトマップの要素数
引数	：float centerHeight：最初の内分点の高さ
引数	：int center		：最初の内分点の添字
戻り値	：void
説明	：2次元ハイトマップ初期化
**************************************************/
void InitializeHeightMap1(float *map, int elemNum, float centerHeight, int center)
{
	map[center] = centerHeight;
	CreateHeightMap1(0, center, 20.0f, map);
	CreateHeightMap1(center, elemNum, 20.0f, map);
}

/*************************************************
関数	：void CreateHeightMap1(int start, int end, float range, float *map)
引数	：int start		：始点
引数	：int end		：終点
引数	：float range	：高さを変異させる範囲
引数	：float *map	：ハイトマップを保存するポインタ
戻り値	：void
説明	：2次元ハイトマップ生成
**************************************************/
void CreateHeightMap1(int start, int end, float range, float *map)
{
	if (abs(end - start) <= 1)
	{
		return;
	}

	int c = start + (end - start) / 2;
	map[c] = (map[end] + map[start]) / 2;
	map[c] += RandomRangef(-range / 2, range / 2);

	CreateHeightMap1(start, c, range / 2, map);
	CreateHeightMap1(c, end, range / 2, map);

	return;
}

/*************************************************
関数	：void InitializeHeightMap1(void)
引数	：float *heightmap	：生成したハイトマップを保存するポインタ
引数	：int elemNum		：ハイトマップの要素数
引数	：float centerHeight：最初の内分点の高さ
引数	：int center		：最初の内分点の添字
戻り値	：void
説明	：3次元ハイトマップ初期化
**************************************************/
void InitializeHeightMap2(float *map, int elemNum)
{
	const float maxHeight = 300.0f;
	const float minHeight = -300.0f;

	Point bottomL = { vtxNum, 0, RandomRangef(minHeight, maxHeight) };
	//map[bottomL.x][bottomL.x] = bottomL.height;
	map[bottomL.x * vtxNum + bottomL.x] = bottomL.height;

	Point bottomR = { vtxNum, vtxNum, RandomRangef(minHeight, maxHeight) };
	//map[bottomR.z][bottomR.x] = bottomR.height;
	map[bottomR.z * vtxNum + bottomR.x] = bottomR.height;

	Point topL = { 0, 0, RandomRangef(minHeight, maxHeight) };
	//map[topL.z][topL.x] = topL.height;
	map[topL.z * vtxNum + topL.x] = topL.height;

	Point topR = { 0, vtxNum, RandomRangef(minHeight, maxHeight) };
	//map[topR.z][topR.x] = topR.height;
	map[topR.z * vtxNum + topR.x] = topR.height;

	Point center = { vtxNum / 2, vtxNum / 2, 1000.0f };
	//map[center.z][center.x] = center.height;
	map[center.z *vtxNum + center.x] = center.height;

	Point bottom = { bottomL.z, center.x, (bottomL.height + bottomR.height) / 2 };
	//map[bottom.z][bottom.x] = bottom.height;
	map[bottom.z * vtxNum + bottom.x] = bottom.height;

	Point top = { topR.z, center.x, (topL.height + topR.height) / 2 };
	//map[top.z][top.x] = top.height;
	map[top.z * vtxNum + top.x] = top.height;

	Point left = { center.z, topL.x, (topL.height + bottomL.height) / 2 };
	//map[left.z][left.x] = left.height;
	map[left.z + vtxNum + left.z] = left.height;

	Point right = { center.z, topR.x, (topR.height + bottomR.height) / 2 };
	//map[right.z][right.x] = right.height;
	map[right.z * vtxNum + right.z] = right.height;

	CreateHeightMap2(map);
}

/*************************************************
関数	：void CreateHeightMap2(Point tr, Point tl, Point br, Point bl, float range, float *map, int elemNum)
引数	：Point tr		：右上のポイント
引数	：Point tl		：左上のポイント
引数	：Point br		：右下のポイント
引数	：Point bl		：左下のポイント
引数	：float range	：変異させる幅
引数	：float *map	：ハイトマップを保存するポインタ
引数	：int elemNum	：ハイトマップの要素数
戻り値	：void
説明	：3次元ハイトマップ生成
**************************************************/
void CreateHeightMap2(float *map)
{

	int i, j, ni, nj, mi, mj, pmi, pmj, rectSize = vtxNum;
	float dh = (float)1000 / 2, r = (float)pow(2, -1 * 0.9);

	//map[0][0] = 0;
	map[0] = 0;

	while (rectSize > 0)
	{
		for (int i = 0; i < vtxNum; i += rectSize)
		{
			for (int j = 0; j < vtxNum; j += rectSize)
			{
				ni = (i + rectSize) % vtxNum;
				nj = (j + rectSize) % vtxNum;

				mi = (i + rectSize / 2);
				mj = (j + rectSize / 2);

				//map[mj][mi] = (map[j][i] + map[j][ni] + map[nj][i] + map[nj][ni]) / 4 + RandomRangef(-dh / 2, dh / 2);
				float average = (map[j * vtxNum + i] + map[j * vtxNum + ni] + map[nj * vtxNum + i] + map[nj * vtxNum + ni]) / 4;
				map[mj * vtxNum + mi] = average + RandomRangef(-dh / 2, dh / 2);
			}
		}

		for (int i = 0; i < vtxNum; i += rectSize)
		{
			for (int j = 0; j < vtxNum; j += rectSize)
			{
				ni = (i + rectSize) % vtxNum;
				nj = (j + rectSize) % vtxNum;

				mi = (i + rectSize / 2);
				mj = (j + rectSize / 2);

				pmi = (i - rectSize / 2 + vtxNum) % vtxNum;
				pmj = (j - rectSize / 2 + vtxNum) % vtxNum;

				//map[j][mi] = (map[j][i] + map[j][ni] + map[pmj][mi] + map[mj][mi]) / 4 + RandomRangef(-dh / 2, dh / 2);
				float average1 = (map[j * vtxNum + i] + map[j * vtxNum + ni] + map[pmj * vtxNum + mi] + map[mj * vtxNum + mi]) / 4;
				map[j * vtxNum + mi] = average1 + RandomRangef(-dh / 2, dh / 2);

				//map[mj][i] = (map[j][i] + map[nj][i] + map[mj][pmi] + map[mj][mi]) / 4 + RandomRangef(-dh / 2, dh / 2);
				float average2 = (map[j * vtxNum + i] + map[nj * vtxNum + i] + map[mj * vtxNum + pmi] + map[mj * vtxNum + mi]) / 4;
				map[mj * vtxNum + i] = average2 + RandomRangef(-dh / 2, dh / 2);
			}
		}

		rectSize /= 2;
		dh *= r;
	}
}