//=====================================
//
//エネミーバレット衝突判定処理[collisionEnemyBullett.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "collisionManager.h"
#include "enemyBullet.h"
#include "playerModel.h"
#include "cameraShaker.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
bool CheckCollisionEnemyBulletLower(DWORD elem, PLAYERMODEL *player, bool isCheckedUpper, COLLISION_MANAGER *manager);
bool CheckCollisionEnemyBulletUpper(DWORD elem, PLAYERMODEL *player, COLLISION_MANAGER *manager);

/**************************************
衝突判定
***************************************/
void CheckCollisionEnemyBullet(COLLISION_MANAGER *manager)
{
	for (DWORD cntCell = 0; cntCell < manager->cellNum; cntCell++)
	{
		//空間が作成されていない場合の判定
		if (!manager->cellArray[OFT_PLAYER][cntCell])
			continue;

		//空間にオブジェクトが登録されていない場合の判定
		if (manager->cellArray[OFT_PLAYER][cntCell]->latestObj == NULL)
			continue;

		//空間に登録されているプレイヤー全てに対して判定
		OBJECT_FOR_TREE *playerOFT = manager->cellArray[OFT_PLAYER][cntCell]->latestObj;
		for (; playerOFT != NULL; playerOFT = playerOFT->next)
		{
			PLAYERMODEL *player = (PLAYERMODEL*)playerOFT->object;
			CheckCollisionEnemyBulletLower(cntCell, player, false, manager);
		}
	}
}

/**************************************
子空間での衝突判定
***************************************/
bool CheckCollisionEnemyBulletLower(DWORD elem, PLAYERMODEL *player, bool isCheckedUpper, COLLISION_MANAGER *manager)
{
	//指定された空間が最大空間数を超えている判定
	if (elem >= manager->cellNum)
		return false;

	//親空間での判定
	if (!isCheckedUpper)
	{
		CheckCollisionEnemyBulletUpper(elem, player, manager);
	}


	//空間が作成されていない場合の判定
	if (!manager->cellArray[OFT_ENEMYBULLET][elem])
		return false;

	//指定された空間内のエネミーバレットにたいして判定
	if (manager->cellArray[OFT_ENEMYBULLET][elem]->latestObj != NULL)
	{
		OBJECT_FOR_TREE *bulletOFT = manager->cellArray[OFT_ENEMYBULLET][elem]->latestObj;
		while (bulletOFT != NULL)
		{
			//衝突判定
			ENEMYBULLET *bullet = (ENEMYBULLET*)bulletOFT->object;
			if (ChechHitBoundingCube(&bullet->collider, &player->collider))
			{
				SetCameraShaker(5.0f);
			}

			//登録されている次のバレットへ
			bulletOFT = bulletOFT->next;
		}

		//子空間でも判定
		for (int i = 0; i < 4; i++)
		{
			DWORD nextElem = elem * 4 + 1 + i;
			CheckCollisionEnemyBulletLower(nextElem, player, true, manager);
		}
	}
	return false;
}

/**************************************
親空間での衝突判定
***************************************/
bool CheckCollisionEnemyBulletUpper(DWORD elem, PLAYERMODEL *player, COLLISION_MANAGER *manager)
{
	for (elem = (elem - 1) >> 2; elem < manager->cellNum; elem = (elem - 1) >> 2)
	{
		//空間が作成されていない場合の判定
		if (!manager->cellArray[OFT_ENEMYBULLET][elem])
			continue;

		//指定された空間内のバレットに対して判定
		if (manager->cellArray[OFT_ENEMYBULLET][elem]->latestObj != NULL)
		{
			OBJECT_FOR_TREE *bulletOFT = manager->cellArray[OFT_ENEMYBULLET][elem]->latestObj;
			while (bulletOFT != NULL)
			{
				//衝突判定
				ENEMYBULLET *bullet = (ENEMYBULLET*)bulletOFT->object;
				if (ChechHitBoundingCube(&bullet->collider, &player->collider))
				{
					SetCameraShaker(5.0f);
				}

				//次のバレットへ
				bulletOFT = bulletOFT->next;
			}
		}
	}
	return false;
}
