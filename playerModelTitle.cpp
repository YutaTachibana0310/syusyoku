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
#define PLAYERTITLE_INITPOS			(D3DXVECTOR3(0.0f, -10.0f, 0.0f))
#define PLAYERTITLE_MOVE_PERIOD		(180)
#define PLAYERTITLE_MOVE_END		(120)
#define PLAYERTITLE_MOVE_RANGE_X	(50.0f)
#define PLAYERTITLE_MOVE_RANGE_Y	(-20.0f)	
#define PLAYERTITLE_SHOTPOS_L		(D3DXVECTOR3(-10.0f, 0.0f, 5.0f))
#define PLAYERTITLE_SHOTPOS_R		(D3DXVECTOR3( 10.0f, 0.0f, 5.0f))
#define PLAYERTITLE_BULLETSPEED		(40.0f)

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
	player->initPos = player->pos;
	targetPos[player->id] = PLAYERTITLE_INITPOS;
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
		targetPos[player->id].x = RandomRangef(-PLAYERTITLE_MOVE_RANGE_X, PLAYERTITLE_MOVE_RANGE_X);
		targetPos[player->id].y = RandomRangef(PLAYERTITLE_MOVE_RANGE_Y, 0.0f);
		player->initPos = player->pos;
		player->cntFrame = 0;
	}

	//移動
	float t = (float)player->cntFrame / (float)PLAYERTITLE_MOVE_END;
	player->pos = EaseInOutCubicVector(t, player->initPos, targetPos[player->id]);

	//回転
	float diff = targetPos[player->id].x - player->pos.x;
	if (diff != 0.0f)
	{
		float x = diff / fabsf(diff);
		player->destRot.z = x * PLAYER_DESTROT_MAX;
	}
	else
	{
		player->destRot.z = 0.0f;
	}

	//ショットポジション更新
	D3DXVec3TransformCoord(&player->shotpos1, &PLAYERTITLE_SHOTPOS_L, &player->mtxWorld);
	D3DXVec3TransformCoord(&player->shotpos2, &PLAYERTITLE_SHOTPOS_R, &player->mtxWorld);

	//攻撃
	if (player->cntFrame % PLAYER_SHOT_INTERBAL == 0 && player->cntFrame > PLAYERTITLE_MOVE_END)
	{
		SetPlayerBullet(player->shotpos1, PLAYERTITLE_BULLETSPEED);
		SetPlayerBullet(player->shotpos2, PLAYERTITLE_BULLETSPEED);
	}

}

/**************************************
退場処理
***************************************/
void ExitPlayerModelTitle(PLAYERMODEL *player)
{

}
