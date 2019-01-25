//=====================================
//
//メモリアロケータ処理[memoryAllocater.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "memoryAllocater.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <windows.h>
#include "debugWindow.h"

/**************************************
マクロ定義
***************************************/
#define MEMORYALLOCATER_LABEL_MAX	(32)	//つけられるラベルの最大文字数

/**************************************
構造体定義
***************************************/
typedef struct _MEMORYINFO {
	char label[MEMORYALLOCATER_LABEL_MAX];	//ラベル
	size_t size;							//確保されたメモリのサイズ
	_MEMORYINFO *next;						//リストの次のメモリ情報
}MEMORYINFO;	//メモリ情報構造体

/**************************************
グローバル変数
***************************************/
static MEMORYINFO *nodeHead = NULL;			//リストしたメモリ情報の先頭

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
終了処理
***************************************/
void UninitMemoryAllocater(void)
{
	//リストしたメモリ情報をすべて解放
	MEMORYINFO *node = nodeHead;
	while (node != NULL)
	{
		MEMORYINFO *tmp = node->next;
		free(node);
		node = tmp;
	}
}

/*************************************
更新処理
***************************************/
void UpdateMemoryAllocater(void)
{
	//メモリ情報を表示
	BeginDebugWindow("Memory Usage");

	for (MEMORYINFO *node = nodeHead; node != NULL; node = node->next)
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

	//メモリを確保
	p = malloc(size);

	if (!p)
	{
		assert(p);
		return NULL;
	}

	//確保したメモリをリストに追加
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

	//解放したことをリストに通知
	OnAllocMemory(size, label, false);
	free(p);
}

/**************************************
メモリ情報更新処理
***************************************/
void OnAllocMemory(size_t size, const char* label, bool isAlloc)
{
	//リストの中からラベルに一致するメモリ情報を検索して更新
	for (MEMORYINFO *node = nodeHead; node != NULL; node = node->next)
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

	//リスト内になかったので新規作成して追加
	MEMORYINFO *newNode = (MEMORYINFO*)malloc(sizeof(MEMORYINFO));
	ZeroMemory(newNode, sizeof(MEMORYINFO));

	strcpy_s(newNode->label, MEMORYALLOCATER_LABEL_MAX, label);
	newNode->size = size;
	newNode->next = nodeHead;
	nodeHead = newNode;
}