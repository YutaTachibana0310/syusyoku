//=====================================
//
//ミドルエネミーアタック処理[middleEnemyAttack.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "middleEnemyModel.h"
#include "enemyHomingBullet.h"

/**************************************
マクロ定義
***************************************/
#define MIDDLEENEMY_ATTACK_INETBAL		(120)					//攻撃間隔
#define MIDDLEENEMY_ATTACK_LOOPMAX		(8)						//1回の攻撃のループ回数
#define MIDDLEENEMY_ATTACK_MAX			(5)						//攻撃終了回数
#define MIDDLEENEMY_ATTACK_ANGLE		(0.765f)				//発射角度
#define MIDDLEENEMY_ATTACK_INITVELOCITY	(200.0f)				//ホーミング弾初期速度
#define MIDDLEENEMY_ATTACK_REACH_MAX	(90)					//ホーミング到達時刻最大値
#define MIDDLEENEMY_ATTACK_REACH_MIN	(80)					//ホーミング到達時刻最小値
#define MIDDLEENEMY_ATTACK_SHOTPOS		(D3DXVECTOR3(0, 0, 50))	//ホーミング弾発射位置

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
void EnterMiddleEnemyAttack(MIDDLEENEMY *enemy)
{
	enemy->cntFrame = 0;
	enemy->atkNum = 0;
}

/**************************************
更新処理
***************************************/
void UpdateMiddleEnemyAttack(MIDDLEENEMY *enemy)
{
	if (enemy->cntFrame % MIDDLEENEMY_ATTACK_INETBAL == 0)
	{
		float baseAngle = D3DXToRadian(360.0f / MIDDLEENEMY_ATTACK_LOOPMAX);

		for (int i = 0; i < MIDDLEENEMY_ATTACK_LOOPMAX; i++)
		{
			D3DXVECTOR3 dir = D3DXVECTOR3(sinf(i * MIDDLEENEMY_ATTACK_ANGLE), cosf(i * MIDDLEENEMY_ATTACK_ANGLE), 0.0f);
			dir *= MIDDLEENEMY_ATTACK_INITVELOCITY;
			SetEnemyHomingBullet(enemy->pos + MIDDLEENEMY_ATTACK_SHOTPOS, dir, RandomRange(MIDDLEENEMY_ATTACK_REACH_MIN, MIDDLEENEMY_ATTACK_REACH_MAX));
		}

		enemy->cntFrame = 0;
		enemy->atkNum++;
	}

	if (enemy->atkNum == MIDDLEENEMY_ATTACK_MAX)
	{
		ChangeStateMiddleEnemy(enemy, MiddleEnemyWait);
	}

	enemy->cntFrame++;
}

/**************************************
退場処理
***************************************/
void ExitMiddleEnemyAttack(MIDDLEENEMY *enemy)
{

}