//=====================================
//
//ボーナスキューブ衝突判定処理[collisionBonusCube.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "collisionManager.h"
#include "bonusCube.h"
#include "playerBullet.h"

/**************************************
マクロ定義
***************************************/
#define  BONUSCUBE_PLAYERBULLET_DAMAGE	(1.0f)
/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
bool CheckCollisionBonusCubeLower(DWORD elem, BONUS_CUBE_OBJECT *bonusCube, bool isCheckedUpper, COLLISION_MANAGER *manager);
bool CheckCollisionBonusCubeUpper(DWORD elem, BONUS_CUBE_OBJECT *bonusCube, COLLISION_MANAGER *manager);

/**************************************
衝突判定
***************************************/
void CheckCollisionBonusCube(COLLISION_MANAGER *manager)
{
	for (DWORD cntCell = 0; cntCell < manager->cellNum; cntCell++)
	{
		//空間が作成されていない場合の判定
		if (!manager->cellArray[OFT_BONUSCUBE][cntCell])
			continue;

		//空間にオブジェクトが登録されていない場合の判定
		if (manager->cellArray[OFT_BONUSCUBE][cntCell]->latestObj == NULL)
			continue;

		//空間に登録されているキューブ全てに対して判定
		OBJECT_FOR_TREE *bonusCubeOFT = manager->cellArray[OFT_BONUSCUBE][cntCell]->latestObj;
		for (; bonusCubeOFT != NULL; bonusCubeOFT = bonusCubeOFT->next)
		{
			BONUS_CUBE_OBJECT *bonusCube = (BONUS_CUBE_OBJECT*)bonusCubeOFT->object;
			CheckCollisionBonusCubeLower(cntCell, bonusCube, false, manager);
		}
	}

	return;
}

/**************************************
子空間での衝突判定
***************************************/
bool CheckCollisionBonusCubeLower(DWORD elem, BONUS_CUBE_OBJECT *bonusCube, bool isCheckedUpper, COLLISION_MANAGER *manager)
{
	//空間数の判定
	if (elem >= manager->cellNum)
		return false;

	//親空間での判定
	if (!isCheckedUpper)
	{
		CheckCollisionBonusCubeUpper(elem, bonusCube, manager);
	}

	//空間が作成されていない場合の判定
	if (!manager->cellArray[OFT_PLAYERBULLET][elem])
		return false;

	//空間に登録されているバレットと判定
	if (manager->cellArray[OFT_PLAYERBULLET][elem]->latestObj != NULL)
	{
		OBJECT_FOR_TREE *bulletOFT = manager->cellArray[OFT_PLAYERBULLET][elem]->latestObj;
		for (; bulletOFT != NULL; bulletOFT = bulletOFT->next)
		{
			PLAYERBULLET *bullet = (PLAYERBULLET*)bulletOFT->object;
			if (ChechHitBoundingCube(&bullet->collider2, &bonusCube->collider))
			{
				BurstPlayerBullet(bullet);
				bonusCube->hp -= BONUSCUBE_PLAYERBULLET_DAMAGE;
			}
		}
	}

	//子空間でも判定
	for (int i = 0; i < 4; i++)
	{
		DWORD nextElem = elem * 4 + 1 + i;
		CheckCollisionBonusCubeLower(nextElem, bonusCube, true, manager);
	}

	return false;
}

/**************************************
親空間での衝突判定
***************************************/
bool CheckCollisionBonusCubeUpper(DWORD elem, BONUS_CUBE_OBJECT *bonusCube, COLLISION_MANAGER *manager)
{
	//ルート空間へたどるまでループ
	for (elem = (elem - 1) >> 2; elem < manager->cellNum; elem = (elem - 1) >> 2)
	{
		//空間が作成されているか判定
		if (!manager->cellArray[OFT_PLAYERBULLET][elem])
			continue;

		if (manager->cellArray[OFT_PLAYERBULLET][elem]->latestObj != NULL)
		{
			OBJECT_FOR_TREE *bulletOFT = manager->cellArray[OFT_PLAYERBULLET][elem]->latestObj;
			while (bulletOFT != NULL)
			{
				PLAYERBULLET *bullet = (PLAYERBULLET*)bulletOFT->object;
				if (ChechHitBoundingCube(&bullet->collider2, &bonusCube->collider))
				{
					BurstPlayerBullet(bullet);
					bonusCube->hp -= BONUSCUBE_PLAYERBULLET_DAMAGE;
				}

				bulletOFT = bulletOFT->next;
			}
		}
	}

	return false;
}