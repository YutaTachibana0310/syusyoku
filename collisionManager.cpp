//=====================================
//
//テンプレート処理[collisionManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "collisionManager.h"
#include "collider.h"

#include "playerBullet.h"
#include "cubeObject.h"
#include "hardCubeObject.h"

/**************************************
マクロ定義
***************************************/
#define COLLISION_DIVISIONSPACE_LEVEL_MAX		(9)			//空間レベルの最大レベル
#define COLLISION_REGION_LEFT					(-500.0f)	//判定空間の左端
#define COLLISION_REGION_RIGHT					(500.0f)	//判定空間の右端
#define COLLISION_REGION_TOP					(-3000.0f)	//判定空間の上端
#define COLLISION_REGION_BOTTOM					(3000.0f)	//判定空間の下端

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static unsigned int spaceNum[COLLISION_DIVISIONSPACE_LEVEL_MAX + 1];	//各レベルの空間数
static DWORD cellNum;					//空間数の総和
static CCell **cellArray[OFT_ID_MAX];	//各オブジェクトが登録される空間の配列

//単位空間の幅
static const float UnitWidth = (COLLISION_REGION_RIGHT - COLLISION_REGION_LEFT) / (1 << COLLISION_DIVISIONSPACE_LEVEL_MAX);

//単位空間の高さ
static const float UnitHeight = (COLLISION_REGION_BOTTOM - COLLISION_REGION_TOP) / (1 << COLLISION_DIVISIONSPACE_LEVEL_MAX);

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

void CheckCollisionPlayerBullet(void);												//プレイヤーバレットの衝突判定
bool CheckCollisionPlayerBulletAndCubeLower(DWORD elem, PLAYERBULLET *bullet);		//プレイヤーバレットとキューブの判定(親空間Ver)
bool CheckCollisionPlayerBulletAndCubeUpper(DWORD elem, PLAYERBULLET *bullet);		//プレイヤーバレットとキューブの判定(子空間Ver)
bool CheckCollisionPlayerBulletAndHardCubeLower(DWORD elem, PLAYERBULLET *bullet);	//プレイヤーバレットとハードキューブの判定(親空間Ver)
bool CheckCollisionPlayerBulletAndHardCubeUpper(DWORD elem, PLAYERBULLET *bullet);	//プレイヤーバレットとハードキューブの判定(子空間Ver)

/**************************************
初期化処理
***************************************/
void InitCollisionManager(int num)
{
	//各レベルの空間数を算出
	spaceNum[0] = 1;
	for (int i = 1; i < COLLISION_DIVISIONSPACE_LEVEL_MAX + 1; i++)
	{
		spaceNum[i] = spaceNum[i - 1] * 4;
	}

	//空間配列のメモリを確保
	cellNum = (spaceNum[COLLISION_DIVISIONSPACE_LEVEL_MAX] * 4 - 1) / 3;
	for (int OFTid = 0; OFTid < OFT_ID_MAX; OFTid++)
	{
		cellArray[OFTid] = (CCell**)malloc(sizeof(CCell*) * cellNum);
		ZeroMemory(cellArray[OFTid], sizeof(CCell*) * cellNum);
	}
}

/**************************************
終了処理
***************************************/
void UninitCollisionManager(int num)
{
	for (int OFTid = 0; OFTid < OFT_ID_MAX; OFTid++)
	{
		free(cellArray[OFTid]);
		cellArray[OFTid] = NULL;
	}
}

/**************************************
更新処理
***************************************/
void UpdateCollisionManager(void)
{
	CheckCollisionPlayerBullet();
}

/**************************************
プレイヤーバレットの衝突判定
***************************************/
void CheckCollisionPlayerBullet(void)
{
	for (DWORD cntCell = 0; cntCell < cellNum; cntCell++)
	{
		//空間が作成されていない場合の判定
		if (!cellArray[OFT_PLAYERBULLET][cntCell])
			continue;

		//空間にオブジェクトが登録されていない場合の判定
		if (cellArray[OFT_PLAYERBULLET][cntCell]->latestObj == NULL)
			continue;

		//空間に登録されているバレット全てに対して判定
		OBJECT_FOR_TREE *playerBulletOFT = cellArray[OFT_PLAYERBULLET][cntCell]->latestObj;
		for( ; playerBulletOFT != NULL; playerBulletOFT = playerBulletOFT->next)
		{
			PLAYERBULLET *playerBullet = (PLAYERBULLET*)playerBulletOFT->object;
			//キューブオブジェクトとの判定
			{
				bool result;
				result = CheckCollisionPlayerBulletAndCubeLower(cntCell, playerBullet);
				if (result)
					continue;
			}
			
			//ハードキューブとの判定
			{
				bool result;
				result = CheckCollisionPlayerBulletAndHardCubeLower(cntCell, playerBullet);
				if (result)
					continue;
			}
		}
	}
}

/**************************************
プレイヤーバレットとキューブオブジェクトの当たり判定
***************************************/
bool CheckCollisionPlayerBulletAndCubeLower(DWORD elem, PLAYERBULLET *bullet)
{
	//指定された空間が最大空間数を超えている判定
	if (elem >= cellNum)
		return false;

	//親空間で判定
	{
		bool result;
		result = CheckCollisionPlayerBulletAndCubeUpper(elem, bullet);
		if (result)
			return true;
	}

	//空間が作成されていない場合の判定
	if (!cellArray[OFT_CUBEOBJECT][elem])
		return false;

	//指定された空間内のキューブオブジェクトに対して判定
	if (cellArray[OFT_CUBEOBJECT][elem]->latestObj != NULL)
	{
		OBJECT_FOR_TREE *cubeOFT = cellArray[OFT_CUBEOBJECT][elem]->latestObj;
		while (cubeOFT != NULL)
		{
			//衝突判定
			CUBE_OBJECT *cube = (CUBE_OBJECT*)cubeOFT->object;
			if (ChechHitBoundingCube(&bullet->collider2, &cube->collider))
			{
				cube->hp -= 1.0f;
				bullet->destroyRequest = true;
				return true;
			}

			//登録されている次のキューブへ
			cubeOFT = cubeOFT->next;
		}
	}

	//子空間でも判定
	for (int i = 0; i < 4; i++)
	{
		DWORD nextElem = elem * 4 + 1 + i;
		bool result = CheckCollisionPlayerBulletAndCubeLower(nextElem, bullet);
		if (!result)
			return true;
	}

	return false;
}

/**************************************
プレイヤーバレットとキューブオブジェクトの当たり判定
***************************************/
bool CheckCollisionPlayerBulletAndCubeUpper(DWORD elem, PLAYERBULLET *bullet)
{
	//指定された空間から親空間へジャンプ
	elem = (elem - 1) >> 2;

	while (elem < cellNum)
	{
		//空間が作成されていない場合の判定
		if (!cellArray[OFT_CUBEOBJECT][elem])
			return false;

		//指定されて空間内のオブジェクトに対して判定
		if (cellArray[OFT_CUBEOBJECT][elem]->latestObj != NULL)
		{
			OBJECT_FOR_TREE *cubeOFT = cellArray[OFT_CUBEOBJECT][elem]->latestObj;
			while (cubeOFT != NULL)
			{
				//衝突判定
				CUBE_OBJECT *cube = (CUBE_OBJECT*)cubeOFT->object;
				if (ChechHitBoundingCube(&bullet->collider2, &cube->collider))
				{
					cube->hp -= 1.0f;
					bullet->destroyRequest = true;
					return false;
				}

				//次のキューブへ
				cubeOFT = cubeOFT->next;
			}
		}

		//指定された空間から親空間へジャンプ
		elem = (elem - 1) >> 2;
	}

	return false;
}

/**************************************
プレイヤーバレットとハードキューブの当たり判定(子空間Ver)
***************************************/
bool CheckCollisionPlayerBulletAndHardCubeLower(DWORD elem, PLAYERBULLET *bullet)
{
	//指定された空間が最大空間数を超えている判定
	if (elem >= cellNum)
		return false;

	//親空間での判定
	{
		bool result;
		result = CheckCollisionPlayerBulletAndHardCubeUpper(elem, bullet);
		if (result)
			return true;
	}
	
	//空間が作成されていない場合の判定
	if (!cellArray[OFT_HARDCUBE][elem])
		return false;

	//指定されて空間内のオブジェクトに対して判定
	if (cellArray[OFT_HARDCUBE][elem]->latestObj != NULL)
	{
		OBJECT_FOR_TREE *hardCubeOFT = cellArray[OFT_HARDCUBE][elem]->latestObj;
		while (hardCubeOFT != NULL)
		{
			//衝突判定
			HARD_CUBE_OBJECT *hardCube = (HARD_CUBE_OBJECT*)hardCubeOFT->object;
			if (ChechHitBoundingCube(&bullet->collider2, &hardCube->collider))
			{
				BurstPlayerBullet(bullet);
				return true;
			}

			//次のキューブへ
			hardCubeOFT = hardCubeOFT->next;
		}
	}

	//子空間でも判定
	for (int i = 0; i < 4; i++)
	{
		DWORD nextElem = elem * 4 + 1 + i;
		bool result;
		result = CheckCollisionPlayerBulletAndHardCubeLower(nextElem, bullet);
		if (result)
			return true;
	}

	return false;
}

/**************************************
プレイヤーバレットとハードキューブの当たり判定(親空間Ver)
***************************************/
bool CheckCollisionPlayerBulletAndHardCubeUpper(DWORD elem, PLAYERBULLET *bullet)
{
	//指定された空間から親空間へジャンプ
	elem = (elem - 1) >> 2;

	while (elem < cellNum)
	{	
		//空間が作成されていない場合の判定
		if (!cellArray[OFT_HARDCUBE][elem])
			return false;

		//指定されて空間内のオブジェクトに対して判定
		if (cellArray[OFT_HARDCUBE][elem]->latestObj != NULL)
		{
			OBJECT_FOR_TREE *hardCubeOFT = cellArray[OFT_HARDCUBE][elem]->latestObj;
			while (hardCubeOFT != NULL)
			{
				//衝突判定
				HARD_CUBE_OBJECT *hardCube = (HARD_CUBE_OBJECT*)hardCubeOFT->object;
				if (ChechHitBoundingCube(&bullet->collider2, &hardCube->collider))
				{
					BurstPlayerBullet(bullet);
					return true;
				}

				//次のキューブへ
				hardCubeOFT = hardCubeOFT->next;
			}
		}

		//指定された空間から親空間へジャンプ
		elem = (elem - 1) >> 2;
	}

	return false;
}

/**************************************
オブジェクト登録処理
***************************************/
//bool RegisterObjectToSpace(float left, float top, float right, float bottom, OBJECT_FOR_TREE *obj, OFT_ID id)
bool RegisterObjectToSpace(COLLIDER_CUBE *collider, OBJECT_FOR_TREE *obj, OFT_ID id)
{
	float left = collider->pos->x - collider->length.x;
	float top = collider->pos->y - collider->length.z;
	float right = collider->pos->x + collider->length.x;
	float bottom = collider->pos->y + collider->length.z;

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
		DWORD check = (def >> (i * 2)) & 0x3;
		if (check != 0)
			hiLevel = i + 1;
	}
	DWORD baseNum = rb >> (hiLevel * 2);
	DWORD addNum = (spaceNum[COLLISION_DIVISIONSPACE_LEVEL_MAX - hiLevel] - 1) / 3;
	DWORD spaceIndex = baseNum + addNum;

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
	return (WORD)(BitSeparate32(x) | (WORD)(BitSeparate32(y) << 1));
}

/**************************************
座標→線形4分木要素番号変換処理
***************************************/
DWORD GetPointElem(float posX, float posY)
{
	return Get2DMortonNumber((WORD)((posX - COLLISION_REGION_LEFT) / UnitWidth), (WORD)((posY - COLLISION_REGION_TOP) / UnitHeight));
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

/**************************************
OBJECT_FOR_TREE作成処理
***************************************/
void CreateOFT(OBJECT_FOR_TREE *oft, void *object)
{
	oft->registerSpace = NULL;
	oft->object = object;
	oft->prev = NULL;
	oft->prev = NULL;
}