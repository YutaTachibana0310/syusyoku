//=====================================
//
//テンプレート処理[template.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModelTitle.h"
#include "playerBullet.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERTITLE_INITPOS		(D3DXVECTOR3(0.0f, -10.0f, 100.0f))
#define PLAYERTITLE_MOVE_PERIOD	(180)
#define PLAYERTITLE_MOVE_END	(120)
#define PLAYERTITLE_MOVE_RANGE	(20.0f)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static D3DXVECTOR3 targetPos[PLAYERMODEL_MAX];

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
入場処理
***************************************/
void EnterPlayerModelTitle(PLAYERMODEL *player)
{
	player->cntFrame = 0;
	player->pos = PLAYERTITLE_INITPOS;

}

/**************************************
更新処理
***************************************/
void UpdatePlayerModelTitle(PLAYERMODEL *player)
{
	player->cntFrame++;

	//移動先を一定周期で設定
	if (player->cntFrame % PLAYERTITLE_MOVE_PERIOD == 0)
	{
		targetPos[player->id] = PLAYERTITLE_INITPOS;
		targetPos[player->id].x = RandomRangef(-PLAYERTITLE_MOVE_RANGE, PLAYERTITLE_MOVE_RANGE);
		player->initPos = player->pos;
		player->cntFrame = 0;
	}

	//移動
	float t = (float)player->cntFrame / (float)PLAYERTITLE_MOVE_END;
	player->pos = EaseInOutCubicVector(t, player->initPos, targetPos[player->id]);

	//回転
	float diff = targetPos[player->id].x - player->pos.x;
	float x = diff / fabsf(diff);
	player->destRot.z = x * PLAYER_DESTROT_MAX;

	//攻撃
	if (player->cntFrame % PLAYER_SHOT_INTERBAL == 0)
	{


	}

}

/**************************************
退場処理
***************************************/
void ExitPlayerModelTitle(PLAYERMODEL *player)
{

}
