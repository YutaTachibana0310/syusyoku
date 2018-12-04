//=====================================
//
//エネミーマネージャ処理[enemyManager.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "enemyManager.h"
#include "enemyMissile.h"
#include "middleEnemyModel.h"
#include "enemyBullet.h"
#include "enemyHomingBullet.h"
#include "enemyBulletTrail.h"

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

/**************************************
初期化処理
***************************************/
void InitEnemyManager(int num)
{
	InitEnemyMissile(num);
	InitMiddleEnemyModel(num);

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
	UninitMiddleEnemyModel(num);

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
	UpdateMiddleEnemyModel();

	UpdateEnemyBullet();
	UpdateEnemyHomingBullet();
	UpdateEnemyBulletTrail();
}

/**************************************
描画処理
***************************************/
void DrawEnemyManager(void)
{
	DrawMiddleEnemyModel();
	DrawEnemyMissile();

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
}