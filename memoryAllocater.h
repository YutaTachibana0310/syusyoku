//=====================================
//
//メモリアロケータヘッダ[memoryAllocater.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _MEMORYALLOCATER_H_
#define _MEMORYALLOCATER_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
void UninitMemoryAllocater(void);
void UpdateMemoryAllocater(void);

void* AllocMemory(size_t size, const char* label);					//メモリ確保処理
void ReleaseMemory(size_t size, const char* label, void *p);		//メモリ解放処理
void OnAllocMemory(size_t size, const char* label, bool isAlloc);	//メモリ情報リストへの通知
#endif