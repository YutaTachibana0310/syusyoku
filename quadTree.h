#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include "main.h"

enum SpaceLevel
{
	GrandSon = 0,
	Child = 2,
	Parent = 4,
	Root,
	SpaceLevelMax
};

DWORD BitSeparate32(DWORD n);
DWORD GetMortonNum(WORD x, WORD y);
SpaceLevel CalcSpaceLevel(DWORD n);
float CalcLoopStartX(SpaceLevel level, DWORD morton);
float CalcLoopEndX(SpaceLevel level, DWORD morton);
float CalcLoopStartZ(SpaceLevel level, DWORD morton);
float CalcLoopEndZ(SpaceLevel level, DWORD morton);
#endif