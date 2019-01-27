//=====================================
//
//ハードキューブ衝突判定処理[collisionHardCube.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "collisionManager.h"
#include "playerBullet.h"
#include "hardCubeObject.h"

/**************************************
マクロ定義
***************************************/
#define HARDCUBE_PLAYERBULLET_DAMAGE	(1.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
bool CheckCollisionHardCubeLower(DWORD elem, HARD_CUBE_OBJECT *hardCube, bool isCheckedUpper, COLLISION_MANAGER *manager);
bool CheckCollisionHardCubeUpper(DWORD elem, HARD_CUBE_OBJECT *hardCube, COLLISION_MANAGER *manager);

/**************************************
衝突判定
***************************************/
void CheckCollisionHardCube(COLLISION_MANAGER *manager)
{
	for (DWORD cntCell = 0; cntCell < manager->cellNum; cntCell++)
	{
		//空間が作成されていない場合の判定
		if (!manager->cellArray[OFT_HARDCUBE][cntCell])
			continue;

		//空間にオブジェクトが登録されていない場合の判定
		if (manager->cellArray[OFT_HARDCUBE][cntCell]->latestObj == NULL)
			continue;

		//空間に登録されているバレット全てに対して判定
		OBJECT_FOR_TREE *hardCubeOFT = manager->cellArray[OFT_HARDCUBE][cntCell]->latestObj;
		for (; hardCubeOFT != NULL; hardCubeOFT = hardCubeOFT->next)
		{
			HARD_CUBE_OBJECT *hardCube = (HARD_CUBE_OBJECT*)hardCubeOFT->object;
			CheckCollisionHardCubeLower(cntCell, hardCube, false, manager);
		}
	}
}

/**************************************
子空間での衝突判定
***************************************/
bool CheckCollisionHardCubeLower(DWORD elem, HARD_CUBE_OBJECT *hardCube, bool isCheckedUpper, COLLISION_MANAGER *manager)
{
	//指定された空間が最大空間数を超えている判定
	if (elem >= manager->cellNum)
		return false;

	//親空間での判定
	if (isCheckedUpper)
	{
		bool result;
		result = CheckCollisionHardCubeUpper(elem, hardCube, manager);
		if (result)
			return true;
	}

	//空間が作成されていない場合の判定
	if (!manager->cellArray[OFT_PLAYERBULLET][elem])
		return false;

	//指定されて空間内のオブジェクトに対して判定
	if (manager->cellArray[OFT_PLAYERBULLET][elem]->latestObj != NULL)
	{
		OBJECT_FOR_TREE *bulletOFT = manager->cellArray[OFT_PLAYERBULLET][elem]->latestObj;
		while (bulletOFT != NULL)
		{
			//衝突判定
			PLAYERBULLET *bullet = (PLAYERBULLET*)bulletOFT->object;
			if (ChechHitBoundingCube(&bullet->collider2, &hardCube->collider))
			{
				BurstPlayerBullet(bullet);
				hardCube->hp -= HARDCUBE_PLAYERBULLET_DAMAGE;
			}

			//次のキューブへ
			bulletOFT = bulletOFT->next;
		}
	}

	//子空間でも判定
	for (int i = 0; i < 4; i++)
	{
		DWORD nextElem = elem * 4 + 1 + i;
		bool result;
		result = CheckCollisionHardCubeLower(nextElem, hardCube, true, manager);
	}

	return false;
}

/**************************************
親空間での衝突判定
***************************************/
bool CheckCollisionHardCubeUpper(DWORD elem, HARD_CUBE_OBJECT *hardCube, COLLISION_MANAGER *manager)
{
	//ルート空間にジャンプするまでループ
	for (elem = (elem - 1) >> 2; elem < manager->cellNum; elem = (elem - 1) >> 2)
	{
		//空間が作成されていない場合の判定
		if (!manager->cellArray[OFT_PLAYERBULLET][elem])
			continue;

		//指定されて空間内のオブジェクトに対して判定
		if (manager->cellArray[OFT_PLAYERBULLET][elem]->latestObj != NULL)
		{
			OBJECT_FOR_TREE *bulletOFT = manager->cellArray[OFT_PLAYERBULLET][elem]->latestObj;
			while (bulletOFT != NULL)
			{
				//衝突判定
				PLAYERBULLET *bullet = (PLAYERBULLET*)bulletOFT->object;
				if (ChechHitBoundingCube(&bullet->collider2, &hardCube->collider))
				{
					BurstPlayerBullet(bullet);
					hardCube->hp -= HARDCUBE_PLAYERBULLET_DAMAGE;
					return true;
				}

				//次のキューブへ
				bulletOFT = bulletOFT->next;
			}
		}
	}

	return false;
}