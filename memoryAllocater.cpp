//=====================================
//
//メモリアロケータ処理[memoryAllocater.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <windows.h>
#include "debugWindow.h"

/**************************************
マクロ定義
***************************************/
#define MEMORYALLOCATER_LABEL_MAX	(32)
/**************************************
構造体定義
***************************************/
typedef struct _MEMORYINFO {
	char label[MEMORYALLOCATER_LABEL_MAX];
	size_t size;
	_MEMORYINFO *next;
}MEMORIINFO;

/**************************************
グローバル変数
***************************************/
static MEMORIINFO *nodeHead = NULL;

/**************************************
プロトタイプ宣言
***************************************/
void OnAllocMemory(size_t size, const char* label, bool isAlloc);

/**************************************
終了処理
***************************************/
void UninitMemoryAllocater(void)
{
	MEMORIINFO *node = nodeHead;
	while (node != NULL)
	{
		MEMORIINFO *tmp = node->next;
		free(node);
		node = tmp;
	}
}

/*************************************
更新処理
***************************************/
void UpdateMemoryAllocater(void)
{
	BeginDebugWindow("Memory Usage");

	for (MEMORIINFO *node = nodeHead; node != NULL; node = node->next)
	{
		DebugText("%s : %d[Byte]", node->label, node->size);
	}

	EndDebugWindow("Memory Usage");
}

/**************************************
メモリ確保処理
***************************************/
void* AllocMemory(size_t size, const char* label)
{
	void *p = NULL;


	p = malloc(size);

	if (!p)
	{
		assert(p);
		return NULL;
	}

	OnAllocMemory(size, label, true);
	return p;
}

/**************************************
メモリ解放処理
***************************************/
void ReleaseMemory(size_t size, const char* label, void *p)
{
	if (p == NULL)
	{
		assert(p);
		return;
	}

	OnAllocMemory(size, label, false);
	free(p);
}

/**************************************
メモリ情報更新処理
***************************************/
void OnAllocMemory(size_t size, const char* label, bool isAlloc)
{
	for (MEMORIINFO *node = nodeHead; node != NULL; node = node->next)
	{
		if (strcmp(node->label, label) == 0)
		{
			if (isAlloc)
				node->size += size;
			else
				node->size -= size;

			return;
		}
	}

	MEMORIINFO *newNode = (MEMORIINFO*)malloc(sizeof(MEMORIINFO));
	ZeroMemory(newNode, sizeof(MEMORIINFO));

	strcpy_s(newNode->label, MEMORYALLOCATER_LABEL_MAX, label);
	newNode->size = size;
	newNode->next = nodeHead;
	nodeHead = newNode;
}