//===========================================================
//
//�t���N�^���v�Z����[fractal.cpp]
//Author�FGP11A341 21 ���ԗY��
//
//===========================================================

#include "fractal.h"
#include "MyLibrary.h"
#include "input.h"

/******************************************************
�\���̒�`
******************************************************/
typedef struct
{
	int z;
	int x;
	float height;
}Point;

/****************************************************
�v���g�^�C�v�錾
****************************************************/
void CreateHeightMap1(int start, int end, float range, float *map);
void CreateHeightMap2(Point tr, Point tl, Point br, Point bl, float range, float *map, int elemNum);

/***************************************************
�O���[�o���ϐ�
***************************************************/
static const float d = 2.0f;
static const float minHeight = -400.0f, maxHeight = 400.0f;

/*************************************************
�֐�	�Fvoid InitializeHeightMap1(void)
����	�Ffloat *heightmap	�F���������n�C�g�}�b�v��ۑ�����|�C���^
����	�Fint elemNum		�F�n�C�g�}�b�v�̗v�f��
����	�Ffloat centerHeight�F�ŏ��̓����_�̍���
����	�Fint center		�F�ŏ��̓����_�̓Y��
�߂�l	�Fvoid
����	�F2�����n�C�g�}�b�v������
**************************************************/
void InitializeHeightMap1(float *map, int elemNum, float centerHeight, int center)
{
	map[center] = centerHeight;
	CreateHeightMap1(0, center, 20.0f, map);
	CreateHeightMap1(center, elemNum, 20.0f, map);
}

/*************************************************
�֐�	�Fvoid CreateHeightMap1(int start, int end, float range, float *map)
����	�Fint start		�F�n�_
����	�Fint end		�F�I�_
����	�Ffloat range	�F������ψق�����͈�
����	�Ffloat *map	�F�n�C�g�}�b�v��ۑ�����|�C���^
�߂�l	�Fvoid
����	�F2�����n�C�g�}�b�v����
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
�֐�	�Fvoid InitializeHeightMap1(void)
����	�Ffloat *heightmap	�F���������n�C�g�}�b�v��ۑ�����|�C���^
����	�Fint elemNum		�F�n�C�g�}�b�v�̗v�f��
����	�Ffloat centerHeight�F�ŏ��̓����_�̍���
����	�Fint center		�F�ŏ��̓����_�̓Y��
�߂�l	�Fvoid
����	�F3�����n�C�g�}�b�v������
**************************************************/
void InitializeHeightMap2(float *map, int elemNum)
{
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
�֐�	�Fvoid CreateHeightMap2(Point tr, Point tl, Point br, Point bl, float range, float *map, int elemNum)
����	�FPoint tr		�F�E��̃|�C���g
����	�FPoint tl		�F����̃|�C���g
����	�FPoint br		�F�E���̃|�C���g
����	�FPoint bl		�F�����̃|�C���g
����	�Ffloat range	�F�ψق����镝
����	�Ffloat *map	�F�n�C�g�}�b�v��ۑ�����|�C���^
����	�Fint elemNum	�F�n�C�g�}�b�v�̗v�f��
�߂�l	�Fvoid
����	�F3�����n�C�g�}�b�v����
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