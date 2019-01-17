//=====================================
//
//エネミーマネージャ処理[enemyManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "enemyManager.h"
#include "enemyMissile.h"
#include "middleEnemyModel.h"
#include "smallEnemyModel.h"
#include "enemyBullet.h"
#include "enemyHomingBullet.h"
#include "enemyBulletTrail.h"
#include "cubeObject.h"
#include "hardCubeObject.h"
#include "bonusCube.h"

/**************************************
マクロ定義
***************************************/
#define CUBEOBJECT_EMMITT_RANGE		(100.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitEnemyManager(int num)
{
	InitEnemyMissile(num);
	InitMiddleEnemy(num);
	InitSmallEnemy(num);
	InitCubeObject(num);
	InitHardCubeObject(num);
	InitBonusCube(num);

	InitEnemyBullet(num);
	InitEnemyHomingBullet(num);
	InitEnemyBulletTrail(num);
}

/**************************************
終了処理
***************************************/
void UninitEnemyManager(int num)
{
	UninitEnemyMissile(num);
	UninitMiddleEnemy(num);
	UninitSmallEnemy(num);
	UninitCubeObject(num);
	UninitHardCubeObject(num);
	UninitBonusCube(num);

	UninitEnemyBullet(num);
	UninitEnemyHomingBullet(num);
	UninitEnemyBulletTrail(num);
}

/**************************************
更新処理
***************************************/
void UpdateEnemyManager(void)
{
	UpdateEnemyMissile();
	UpdateMiddleEnemy();
	UpdateSmallEnemy();
	UpdateCubeObject();
	UpdateHardCubeObject();
	UpdateBonusCube();

	UpdateEnemyBullet();
	UpdateEnemyHomingBullet();
	UpdateEnemyBulletTrail();
}

/**************************************
描画処理
***************************************/
void DrawEnemyManager(void)
{
	DrawMiddleEnemy();
	DrawEnemyMissile();
	DrawSmallEnemy();
	DrawCubeObject();
	DrawHardCubeObject();
	DrawBonusCube();

	DrawEnemyBullet();
	DrawEnemyHomingBullet();
	DrawEnemyBulletTrail();
}

/**************************************
当たり判定処理
***************************************/
void CheckEnemyCollision(void)
{
	LockonMiddleEnemy();
	LockonEnemyMissile();
	LockonSmallEnemy();
	LockonBonusCube();
	LockonHardCubeObject();
	LockonCubeObject();

	CollisionMiddleEnemyAndBullet();
	CollisionSmallEnemyAndBullet();
	//CollisionCubeObjectAndBullet();
}

/**************************************
キューブオブジェクト発生処理
***************************************/
void EmmittCubeObject(int num, D3DXVECTOR3 *setPos, float setSpeed)
{
	for (int i = 0; i < num; i++)
	{
		D3DXVECTOR3 offsetPos;
		offsetPos.x = RandomRangef(-CUBEOBJECT_EMMITT_RANGE, CUBEOBJECT_EMMITT_RANGE);
		offsetPos.y = RandomRangef(-CUBEOBJECT_EMMITT_RANGE, CUBEOBJECT_EMMITT_RANGE);
		offsetPos.z = RandomRangef(-CUBEOBJECT_EMMITT_RANGE, CUBEOBJECT_EMMITT_RANGE);

		bool resultEmmitt = SetCubeObject(&(*setPos + offsetPos), setSpeed);
		if (!resultEmmitt)
			break;
	}
}

/**************************************
ボーナスキューブ発生処理
***************************************/
void EmmittBonusCube(D3DXVECTOR3 *setPos)
{
	//テロップ表示とか

	SetBonusCube(setPos);
}

/**************************************
ハードキューブ発生処理
***************************************/
void EmittHardCubeObject(int num, D3DXVECTOR3 *setPos)
{
	for (int i = 0; i < num; i++)
	{
		SetHardCubeObject(setPos);
	}
}