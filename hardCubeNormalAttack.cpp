//=====================================
//
//ハードキューブ通常攻撃処理[hardCubeNormalAttack.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "hardCubeObject.h"
#include "playerModel.h"
#include "enemyBullet.h"

/**************************************
マクロ定義
***************************************/
#define HARDCUBE_NORMALATTACK_INTERBAL		(60)
#define HARDCUBE_NORMALATTACK_DURATION		(HARDCUBE_NORMALATTACK_INTERBAL*5)
#define HARDCUBE_NORNALATTACL_BULLETSPEED	(10.0f)

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
void OnEnterHardCubeNormalAttack(HARD_CUBE_OBJECT *ptr)
{
	ptr->cntFrame = 0;
}

/**************************************
更新処理
***************************************/
void OnUpdateHardCubeNormalAttack(HARD_CUBE_OBJECT *ptr)
{
	ptr->cntFrame++;
	if (ptr->cntFrame % HARDCUBE_NORMALATTACK_INTERBAL == 0)
	{
		D3DXVECTOR3 dir = GetPlayerAdr(0)->pos - ptr->pos;
		D3DXVec3Normalize(&dir, &dir);
		SetEnemyBullet(ptr->pos, dir, HARDCUBE_NORNALATTACL_BULLETSPEED);
	}

	if (ptr->cntFrame == HARDCUBE_NORMALATTACK_DURATION)
		ChangeStateHardCube(ptr, HardCubeEscape);
}