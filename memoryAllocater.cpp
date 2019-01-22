//=====================================
//
//メモリアロケータ処理[memoryAllocater.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
static MEMORIINFO *memInfo = NULL;

/**************************************
プロトタイプ宣言
***************************************/

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

	return p;
}

/**************************************
メモリ解放処理
***************************************/
void ReleaseMemory(void *p)
{
	free(p);
}