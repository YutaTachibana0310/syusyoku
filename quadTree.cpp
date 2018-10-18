#include "quadTree.h"

/*
対象を1ビット間隔に分割する関数
*/
DWORD BitSeparate32(DWORD n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	n = (n | (n << 1)) & 0x55555555;

	return n;
}

/*
モートン番号（分割したフィールドの通し番号）を算出する関数
引数1：X座標
引数2：Y座標
*/
DWORD GetMortonNum(WORD x, WORD y)
{
	return (BitSeparate32(x) | BitSeparate32(y) << 1);
}

/*
空間レベル計算関数
引数：対象のモートン番号の排他的論理和
*/
SpaceLevel CalcSpaceLevel(DWORD n)
{
	int cnt = -1;
	for (int i = 0; i < 6; i++)
	{
		if ((n & 0x0001) == 1)
		{
			cnt = i;
		}

		n = n >> 1;
	}

	if (cnt == -1)
		return GrandSon;

	if (cnt < 2)
		return Child;

	if (cnt < 4)
		return Parent;

	else
		return Root;
}

int CalcSpaceIndex(SpaceLevel level, DWORD morton)
{
	morton = morton >> level;

	return (morton & 0x0003);
}

/*
ループ始点計算処理（X軸）
*/
float CalcLoopStartX(SpaceLevel level, DWORD morton)
{
	float res = 0.0f;

	switch (level)
	{
	case GrandSon:
		res += (CalcSpaceIndex(Parent, morton) & 0x0001) * 0.5f;
		res += (CalcSpaceIndex(Child, morton) & 0x0001) * 0.25f;
		res += (CalcSpaceIndex(GrandSon, morton) & 0x0001) * 0.125f;
		break;

	case Child:
		res += (CalcSpaceIndex(Parent, morton) & 0x0001) * 0.5f;
		res += (CalcSpaceIndex(Child, morton) & 0x0001) * 0.25f;
		break;


	case Parent:
		res += (CalcSpaceIndex(Parent, morton) & 0x0001) * 0.5f;
		break;


	default:
		res = 0.0f;
		break;
	}

	return res;
}

/*
ループ終点計算処理（X軸）
*/
float CalcLoopEndX(SpaceLevel level, DWORD morton)
{
	float res = 0.0f;

	switch (level)
	{
	case GrandSon:
		res += (CalcSpaceIndex(Parent, morton) & 0x0001) * 0.5f;
		res += (CalcSpaceIndex(Child, morton) & 0x0001) * 0.25f;
		res += (CalcSpaceIndex(GrandSon, morton) & 0x0001) * 0.125f;
		res += 0.125f;
		break;

	case Child:
		res += (CalcSpaceIndex(Parent, morton) & 0x0001) * 0.5f;
		res += (CalcSpaceIndex(Child, morton) & 0x0001) * 0.25f;
		res += 0.25f;
		break;


	case Parent:
		res += (CalcSpaceIndex(Parent, morton) & 0x0001) * 0.5f;
		res += 0.5f;
		break;


	default:
		res = 1.0f;
		break;
	}

	return res;
}

/*
ループ始点計算処理（X軸）
*/
float CalcLoopStartZ(SpaceLevel level, DWORD morton)
{
	float res = 0.0f;

	switch (level)
	{
	case GrandSon:
		res += ((CalcSpaceIndex(Parent, morton) >> 1) & 0x0001) * 0.5f;
		res += ((CalcSpaceIndex(Child, morton) >> 1) & 0x0001) * 0.25f;
		res += ((CalcSpaceIndex(GrandSon, morton) >> 1) & 0x0001) * 0.125f;
		break;

	case Child:
		res += ((CalcSpaceIndex(Parent, morton) >> 1) & 0x0001) * 0.5f;
		res += ((CalcSpaceIndex(Child, morton) >> 1) & 0x0001) * 0.25f;
		break;

	case Parent:
		res += ((CalcSpaceIndex(Parent, morton) >> 1) & 0x0001) * 0.5f;
		break;

	default:
		res = 0.0f;
		break;
	}

	return res;
}

/*
ループ終点計算処理（X軸）
*/
float CalcLoopEndZ(SpaceLevel level, DWORD morton)
{
	float res = 0.0f;

	switch (level)
	{
	case GrandSon:
		res += ((CalcSpaceIndex(Parent, morton) >> 1) & 0x0001) * 0.5f;
		res += ((CalcSpaceIndex(Child, morton) >> 1) & 0x0001) * 0.25f;
		res += ((CalcSpaceIndex(GrandSon, morton) >> 1) & 0x0001) * 0.125f;
		res += 0.125f;
		break;

	case Child:
		res += ((CalcSpaceIndex(Parent, morton) >> 1) & 0x0001) * 0.5f;
		res += ((CalcSpaceIndex(Child, morton) >> 1) & 0x0001) * 0.25f;
		res += 0.25f;
		break;

	case Parent:
		res += ((CalcSpaceIndex(Parent, morton) >> 1) & 0x0001) * 0.5f;
		res += 0.5f;
		break;

	default:
		res = 1.0f;
		break;
	}

	return res;
}