//=====================================
//
//キューブオブジェクト衝突判定処理[collisionCubeObject.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "collisionManager.h"
#include "cubeObject.h"
#include "playerBullet.h"

/**************************************
マクロ定義
***************************************/
#define CUBEOBJ_PLAYERBULLET_DAMAGE		(1.0f)
#define USE_DIVIDESPACE
/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static bool useDivideSpace;

/**************************************
プロトタイプ宣言
***************************************/
bool CheckCollisionCubeObjLower(DWORD elem, PLAYERBULLET *bullet, bool isCheckUpper, COLLISION_MANAGER *manager);
bool CheckCollisionCubeObjUpper(DWORD elem, PLAYERBULLET *bullet, COLLISION_MANAGER *manager);
void CheckCollisionAllCubeAndBullet(void);

/**************************************
衝突判定
***************************************/
void CheckCollisionCubeObject(COLLISION_MANAGER *manager)
{
	//四分木空間分割を使う場合
	if (useDivideSpace)
	{
		for (DWORD cntCell = 0; cntCell < manager->cellNum; cntCell++)
		{
			//空間が作成されていない場合の判定
			if (!manager->cellArray[OFT_PLAYERBULLET][cntCell])
				continue;

			//空間にオブジェクトが登録されていない場合の判定
			if (manager->cellArray[OFT_PLAYERBULLET][cntCell]->latestObj == NULL)
				continue;

			//空間に登録されているバレット全てに対して判定
			OBJECT_FOR_TREE *playerBulletOFT = manager->cellArray[OFT_PLAYERBULLET][cntCell]->latestObj;
			for (; playerBulletOFT != NULL; playerBulletOFT = playerBulletOFT->next)
			{
				PLAYERBULLET *playerBullet = (PLAYERBULLET*)playerBulletOFT->object;
				//キューブオブジェクトとの判定
				{
					bool result;
					result = CheckCollisionCubeObjLower(cntCell, playerBullet, false, manager);
					if (result)
						continue;
				}
			}
		}
	}
	//空間分割を使わない場合
	else
	{
		CheckCollisionAllCubeAndBullet();
	}
}

/**************************************
子空間との衝突判定
**************************************/
bool CheckCollisionCubeObjLower(DWORD elem, PLAYERBULLET *bullet, bool isCheckedUpper, COLLISION_MANAGER *manager)
{
	//指定された空間が最大空間数を超えている判定
	if (elem >= manager->cellNum)
		return false;

	//親空間で判定
	if (!isCheckedUpper)
	{
		bool result;
		result = CheckCollisionCubeObjUpper(elem, bullet, manager);
		if (result)
			return true;
	}

	//空間が作成されていない場合の判定
	if (!manager->cellArray[OFT_CUBEOBJECT][elem])
		return false;

	//指定された空間内のキューブオブジェクトに対して判定
	if (manager->cellArray[OFT_CUBEOBJECT][elem]->latestObj != NULL)
	{
		OBJECT_FOR_TREE *cubeOFT = manager->cellArray[OFT_CUBEOBJECT][elem]->latestObj;
		while (cubeOFT != NULL)
		{
			//衝突判定
			CUBE_OBJECT *cube = (CUBE_OBJECT*)cubeOFT->object;
			//cntCheck++;
			if (ChechHitBoundingCube(&bullet->collider2, &cube->collider))
			{
				cube->hp -= CUBEOBJ_PLAYERBULLET_DAMAGE;
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
		bool result = CheckCollisionCubeObjLower(nextElem, bullet, true, manager);
		if (!result)
			return true;
	}

	return false;
}

/*************************************
親空間との衝突判定
**************************************/
bool CheckCollisionCubeObjUpper(DWORD elem, PLAYERBULLET *bullet, COLLISION_MANAGER *manager)
{
	for (elem = (elem - 1) >> 2; elem < manager->cellNum; elem = (elem - 1) >> 2)
	{
		//空間が作成されていない場合の判定
		if (!manager->cellArray[OFT_CUBEOBJECT][elem])
			continue;;

		//指定されて空間内のオブジェクトに対して判定
		if (manager->cellArray[OFT_CUBEOBJECT][elem]->latestObj != NULL)
		{
			OBJECT_FOR_TREE *cubeOFT = manager->cellArray[OFT_CUBEOBJECT][elem]->latestObj;
			while (cubeOFT != NULL)
			{
				//衝突判定
				CUBE_OBJECT *cube = (CUBE_OBJECT*)cubeOFT->object;
				//cntCheck++;
				if (ChechHitBoundingCube(&bullet->collider2, &cube->collider))
				{
					cube->hp -= CUBEOBJ_PLAYERBULLET_DAMAGE;
					bullet->destroyRequest = true;
					return false;
				}

				//次のキューブへ
				cubeOFT = cubeOFT->next;
			}
		}
	}

	return false;
}

/*************************************
総当たりでの判定
**************************************/
void CheckCollisionAllCubeAndBullet(void)
{
	CUBE_OBJECT *cube = GetCubeObjectAdr(0);
	PLAYERBULLET *bullet = NULL;

	for (int i = 0; i < CUBEOBJECT_NUM_MAX; i++, cube++)
	{
		if (!cube->active)
			continue;

		bullet = GetPlayerBulletAdr(0);
		for (int j = 0; j < PLAYERBULLET_MAX; j++, bullet++)
		{
			if (!bullet->active)
				continue;

			if (ChechHitBoundingCube(&cube->collider, &bullet->collider2))
			{
				cube->hp -= CUBEOBJ_PLAYERBULLET_DAMAGE;
				bullet->destroyRequest = true;
			}
		}
	}
}

/*************************************
空間分割の使用切り替え処理
**************************************/
void SetUseDivideSpace(void)
{
	useDivideSpace = !useDivideSpace;
}