//=====================================
//
//テンプレート処理[collisionManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "collisionManager.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/
#define COLLISION_DIVISIONSPACE_LEVEL_MAX		(9)			//空間レベルの最大レベル
#define COLLISION_REGION_LEFT					(-500.0f)	//判定空間の左端
#define COLLISION_REGION_RIGHT					(500.0f)	//判定空間の右端
#define COLLISION_REGION_TOP					(5000.0f)	//判定空間の上端
#define COLLISION_REGION_BOTTOM					(-5000.0f)	//判定空間の下端

//単位分割空間の幅
#define COLLISION_UNIT_WIDTH ((COLLISION_REGION_RIGHT - COLLISION_REGION_LEFT) / (1 << COLLISION_DIVISIONSPACE_LEVEL_MAX))

//単位分割空間の高さ
#define COLLISION_UNIT_HEIGHT ((COLLISION_REGION_TOP - COLLISION_REGION_BOTTOM) / (1 << COLLISION_DIVISIONSPACE_LEVEL_MAX))

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static unsigned int spaceNum[COLLISION_DIVISIONSPACE_LEVEL_MAX + 1];	//各レベルの空間数
static DWORD cellNum;					//空間数の総和
static CCell **cellArray[OFT_ID_MAX];	//各オブジェクトが登録される空間の配列

/**************************************
プロトタイプ宣言
***************************************/
bool PushObjectToList(CCell *space, OBJECT_FOR_TREE *obj);					//リストへのオブジェクト登録処理
bool OnObjectRemoveFromList(CCell *space, OBJECT_FOR_TREE *obj);			//空間へのオブジェクト離脱通知処理
bool CreateNewCell(DWORD elem, OFT_ID OFTid);								//空間作成処理
DWORD GetMortonNumber(float left, float top, float right, float bottom);	//空間番号算出処理
DWORD BitSeparate32(DWORD n);												//ビット分割処理
WORD Get2DMortonNumber(WORD x, WORD y);										//2Dモートン番号算出処理
DWORD GetPointElem(float posX, float posY);									//座標→線形4分木要素番号変換処理

/**************************************
初期化処理
***************************************/
void InitCollisionManager(int num)
{

}

/**************************************
終了処理
***************************************/
void UninitCollisionManager(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdateCollisionManager(void)
{

}

/**************************************
オブジェクト登録処理
***************************************/
bool RegisterObjectToSpace(float left, float top, float right, float bottom, OBJECT_FOR_TREE *obj, OFT_ID id)
{
	DWORD elem = GetMortonNumber(left, top, right, bottom);
	if (elem < cellNum)
	{
		//空間が未割り当ての場合は新規作成
		if (cellArray[id][elem] == NULL)
			CreateNewCell(elem, id);

		return PushObjectToList(cellArray[id][elem], obj);
	}

	return false;
}

/**************************************
空間作成処理
***************************************/
bool CreateNewCell(DWORD elem, OFT_ID OFTid)
{
	//空間を作成後、ルート空間に辿り着くまで続ける
	while (!cellArray[OFTid][elem])
	{
		//指定の空間を作成
		cellArray[OFTid][elem] = (CCell*)malloc(sizeof(CCell));
		cellArray[OFTid][elem]->latestObj = NULL;

		//親空間にジャンプ
		elem = (elem - 1) >> 2;

		if (elem >= cellNum)
			break;
	}

	return true;
}
/**************************************
空間番号算出処理
***************************************/
DWORD GetMortonNumber(float left, float top, float right, float bottom)
{
	//境界図形の2Dモートン番号を算出
	DWORD lt = GetPointElem(left, top);
	DWORD rb = GetPointElem(right, bottom);

	//所属レベルを算出
	DWORD def = rb ^ lt;
	unsigned int hiLevel = 0;
	for (unsigned int i = 0; i < COLLISION_DIVISIONSPACE_LEVEL_MAX; i++)
	{
		DWORD check = (def >> (i * 2)) & 0x03;
		if (check != 0)
			hiLevel = i + 1;
	}
	DWORD spaceIndex = rb >> (hiLevel * 2);
	DWORD addNum = (spaceNum[COLLISION_DIVISIONSPACE_LEVEL_MAX - hiLevel] - 1) / 3;
	spaceIndex += addNum;

	if (spaceIndex > cellNum)
		return 0xffffffff;

	return spaceIndex;
}

/**************************************
ビット演算処理
***************************************/
DWORD BitSeparate32(DWORD n)
{
	n = (n | (n << 8)) & 0x00ff00ff;
	n = (n | (n << 4)) & 0x0f0f0f0f;
	n = (n | (n << 2)) & 0x33333333;
	return (n | (n << 1)) & 0x55555555;
}

/**************************************
2Dモートン番号算出処理
***************************************/
WORD Get2DMortonNumber(WORD x, WORD y)
{
	return (WORD)(BitSeparate32(x) | (BitSeparate32(y) << 1));
}

/**************************************
座標→線形4分木要素番号変換処理
***************************************/
DWORD GetPointElem(float posX, float posY)
{
	return Get2DMortonNumber((WORD)((posX - COLLISION_REGION_LEFT) / COLLISION_UNIT_WIDTH), (WORD)((posY + COLLISION_REGION_TOP) / COLLISION_UNIT_HEIGHT));
}

/**************************************
空間からオブジェクトを取り除く処理
***************************************/
bool RemoveObjectFromSpace(OBJECT_FOR_TREE *obj)
{
	//すでに取り除かれている場合の判定
	if (obj->registerSpace == NULL)
		return false;

	//登録されている空間に離脱を通知
	if (!OnObjectRemoveFromList(obj->registerSpace, obj))
		return false;

	//前後のオブジェクトを連結する
	if (obj->prev != NULL)
	{
		obj->prev->next = obj->next;
	}
	if (obj->next != NULL)
	{
		obj->next->prev = obj->prev;
	}

	//離脱
	obj->prev = NULL;
	obj->next = NULL;
	obj->registerSpace = NULL;
	return true;
}

/**************************************
リストへのオブジェクト登録処理
***************************************/
bool PushObjectToList(CCell *space, OBJECT_FOR_TREE *obj)
{
	//NULLチェック
	if (obj == NULL)
		return false;

	//二重登録チェック
	if (obj->registerSpace == space)
		return false;

	//新規登録
	if (space->latestObj == NULL)
	{
		space->latestObj = obj;
	}
	//追加登録
	else
	{
		obj->next = space->latestObj;
		space->latestObj->prev = obj;
		space->latestObj = obj;
	}

	//空間を登録
	obj->registerSpace = space;
	return true;
}

/**************************************
離脱オブジェクトのチェック処理
***************************************/
bool OnObjectRemoveFromList(CCell *space, OBJECT_FOR_TREE *obj)
{
	//削除されるオブジェクトが先頭の場合、挿げ替え
	if (space->latestObj == obj)
	{
		if (space->latestObj != NULL)
			space->latestObj = space->latestObj->next;
	}
	return true;
}