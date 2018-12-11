//=====================================
//
//スモールエネミー攻撃処理[smallEnemyAttack.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "smallEnemyModel.h"
#include "enemyBullet.h"
#include "playerModel.h"

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
入場処理
***************************************/
void EnterSmallEnemyAttack(SMALLENEMY *enemy)
{
	//攻撃処理
	SetEnemyBullet(enemy->pos, GetPlayerAdr(0)->pos - enemy->pos, 50.0f);

	ChangeStateSmallEnemy(enemy, SmallEnemyEscape);
}

/**************************************
更新処理
***************************************/
void UpdateSmallEnemyAttack(SMALLENEMY *enemy)
{

}

/**************************************
退場処理
***************************************/
void ExitSmallEnemyAttack(SMALLENEMY *enemy)
{

}