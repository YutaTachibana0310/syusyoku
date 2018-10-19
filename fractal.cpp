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
void CreateHeightMap2(Point tr, Point tl, Point br, Point bl, float range, float *map, int elemNum);

void InitializeHeightMap2o(void);
void CreateHeightMap2o(Point tr, Point tl, Point br, Point bl, float range);
/***************************************************
グローバル変数
***************************************************/
static const float d = 2.0f;
static const float minHeight = -400.0f, maxHeight = 400.0f;

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
	map[c] += RandomRange(-range / 2, range / 2);

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
	InitializeHeightMap2o();

	Point bottomL = { elemNum, 0, RandomRange(minHeight, maxHeight) };
	bottomL.height = Clampf(minHeight, maxHeight, bottomL.height);
	map[bottomL.z * (elemNum + 1) + bottomL.x] = bottomL.height;

	Point bottomR = { elemNum, elemNum, RandomRange(minHeight, maxHeight) };
	bottomR.height = Clampf(minHeight, maxHeight, bottomR.height);
	map[bottomR.z * (elemNum + 1) + bottomR.x] = bottomR.height;

	Point topL = { 0, 0, RandomRange(minHeight, maxHeight) };
	topL.height = Clampf(minHeight, maxHeight, topL.height);
	map[topL.z * (elemNum + 1) + topL.x] = topL.height;

	Point topR = { 0, elemNum, RandomRange(minHeight, maxHeight) };
	topR.height = Clampf(minHeight, maxHeight, topR.height);
	map[topR.z * (elemNum + 1) + topR.x] = topR.height;

	Point center = { elemNum / 2, elemNum / 2, maxHeight };
	center.height = Clampf(minHeight, maxHeight, center.height);
	map[center.z * (elemNum + 1) + center.x] = center.height;

	Point bottom = { bottomL.z, center.x, (bottomL.height + bottomR.height) / 2.0f };
	bottom.height = Clampf(minHeight, maxHeight, bottom.height);
	map[bottom.z * (elemNum + 1) + bottom.x] = bottom.height;

	Point top = { topR.z, center.x, (topL.height + topR.height) / 2.0f };
	top.height = Clampf(minHeight, maxHeight, top.height);
	map[top.z * (elemNum + 1) + top.x] = top.height;

	Point left = { center.z, topL.x, (topL.height + bottomL.height) / 2.0f };
	left.height = Clampf(minHeight, maxHeight, left.height);
	map[left.z * (elemNum + 1) + left.x] = left.height;

	Point right = { center.z, topR.x, (topR.height + bottomR.height) / 2.0f };
	right.height = Clampf(minHeight, maxHeight, right.height);
	map[right.z * (elemNum + 1) + right.x] = right.height;

	float initRand = 2000.0f;

	CreateHeightMap2(top, topL, center, left, initRand, map, elemNum);
	CreateHeightMap2(topR, top, right, center, initRand, map, elemNum);
	CreateHeightMap2(center, left, bottom, bottomL, initRand, map, elemNum);
	CreateHeightMap2(right, center, bottomR, bottom, initRand, map, elemNum);

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
void CreateHeightMap2(Point tr, Point tl, Point br, Point bl, float range, float *map, int elemNum)
{
	if (abs(tr.x - tl.x) <= 1 || abs(br.z - tr.z) <= 1)
	{
		return;
	}

	int z = (br.z + tr.z) / 2;
	int x = (tr.x + tl.x) / 2;
	map[z * (elemNum + 1) + x] = (tr.height + tl.height + br.height + bl.height) / 4.0f;
	map[z * (elemNum + 1) + x] += RandomRange(-range / 2.0f, range / 2.0f);
	map[z * (elemNum + 1) + x] = Clampf(minHeight, maxHeight, map[z * (elemNum + 1) + x]);
	Point c = { z, x, map[z * (elemNum + 1) + x] };

	Point t = { tl.z, c.x, (tl.height + tr.height) / 2.0f };
	t.height = Clampf(minHeight, maxHeight, t.height);
	map[t.z * (elemNum + 1) + t.x] = t.height;

	Point b = { bl.z, c.x, (bl.height + br.height) / 2.0f };
	b.height = Clampf(minHeight, maxHeight, b.height);
	map[b.z * (elemNum + 1) + b.x] = b.height;

	Point r = { c.z, tr.x, (tr.height + br.height) / 2.0f };
	r.height = Clampf(minHeight, maxHeight, r.height);
	map[r.z * (elemNum + 1) + r.x] = r.height;

	Point l = { c.z, bl.x, (tl.height + bl.height) / 2.0f };
	l.height = Clampf(minHeight, maxHeight, l.height);
	map[l.z * (elemNum + 1) + l.x] = l.height;

	CreateHeightMap2(t, tl, c, l, range / d, map, elemNum);
	CreateHeightMap2(tr, t, r, c, range / d, map, elemNum);
	CreateHeightMap2(c, l, b, bl, range / d, map, elemNum);
	CreateHeightMap2(r, c, br, b, range / d, map, elemNum);
	return;
}

static float heightMap2[129][129];
static float heightMap2Inv[129][129];

void InitializeHeightMap2o(void)
{
	Point bottomL = { MESH_BLOCK_NUM, 0, RandomRange(0.0f, maxHeight) };
	heightMap2[bottomL.x][bottomL.x] = bottomL.height;

	Point bottomR = { MESH_BLOCK_NUM, MESH_BLOCK_NUM, RandomRange(0.0f, maxHeight) };
	heightMap2[bottomR.z][bottomR.x] = bottomR.height;

	Point topL = { 0, 0, RandomRange(0.0f, maxHeight) };
	heightMap2[topL.z][topL.x] = topL.height;

	Point topR = { 0, MESH_BLOCK_NUM, RandomRange(0.0f, maxHeight) };
	heightMap2[topR.z][topR.x] = topR.height;

	Point center = { MESH_BLOCK_NUM / 2, MESH_BLOCK_NUM / 2, maxHeight };
	heightMap2[center.z][center.x] = center.height;

	Point bottom = { bottomL.z, center.x, (bottomL.height + bottomR.height) / 2 };
	heightMap2[bottom.z][bottom.x] = bottom.height;

	Point top = { topR.z, center.x, (topL.height + topR.height) / 2 };
	heightMap2[top.z][top.x] = top.height;

	Point left = { center.z, topL.x, (topL.height + bottomL.height) / 2 };
	heightMap2[left.z][left.x] = left.height;

	Point right = { center.z, topR.x, (topR.height + bottomR.height) / 2 };
	heightMap2[right.z][right.x] = right.height;

	float initRand = 40000.0f;

	CreateHeightMap2o(top, topL, center, left, initRand);
	CreateHeightMap2o(topR, top, right, center, initRand);
	CreateHeightMap2o(center, left, bottom, bottomL, initRand);
	CreateHeightMap2o(right, center, bottomR, bottom, initRand);

	for (int i = 0; i < MESH_BLOCK_NUM + 1; i++)
	{
		for (int j = 0; j < MESH_BLOCK_NUM + 1; j++)
		{
			heightMap2Inv[i][j] = heightMap2[MESH_BLOCK_NUM - i][j];
		}
	}
}

void CreateHeightMap2o(Point tr, Point tl, Point br, Point bl, float range)
{
	if (abs(tr.x - tl.x) <= 1 && abs(br.z - tr.z) <= 1)
	{
		return;
	}

	int z = (br.z + tr.z) / 2;
	int x = (tr.x + tl.x) / 2;
	heightMap2[z][x] = (tr.height + tl.height + br.height + bl.height) / 4;
	heightMap2[z][x] += RandomRange(-range / 2, range / 2);
	heightMap2[z][x] = Clampf(minHeight, maxHeight, heightMap2[z][x]);
	Point c = { z, x, heightMap2[z][x] };

	Point t = { tl.z, c.x, (tl.height + tr.height) / 2 };
	t.height = Clampf(minHeight, maxHeight, t.height);
	heightMap2[t.z][t.x] = t.height;

	Point b = { bl.z, c.x, (bl.height + br.height) / 2 };
	b.height = Clampf(minHeight, maxHeight, b.height);
	heightMap2[b.z][b.x] = b.height;

	Point r = { c.z, tr.x, (tr.height + br.height) / 2 };
	r.height = Clampf(minHeight, maxHeight, r.height);
	heightMap2[r.z][r.x] = r.height;

	Point l = { c.z, tl.x, (tl.height + bl.height) / 2 };
	l.height = Clampf(minHeight, maxHeight, l.height);
	heightMap2[l.z][l.x] = l.height;

	CreateHeightMap2o(t, tl, c, l, range / d);
	CreateHeightMap2o(tr, t, r, c, range / d);
	CreateHeightMap2o(c, l, b, bl, range / d);
	CreateHeightMap2o(r, c, br, b, range / d);

	return;
}

float *getHeightMap2(void)
{
	static int cnt = 0;
	return (cnt++ & 1) ? &heightMap2[0][0] : &heightMap2Inv[0][0];
}