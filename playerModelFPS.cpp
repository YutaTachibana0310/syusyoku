//=====================================
//
//プレイヤーモデル一人称処理[playerModelFPS.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModelFPS.h"
#include "playerBullet.h"
#include "input.h"
#include "targetSite.h"
#include "enemyMissile.h"
#include "playerMissile.h"
#include "lockonGUI.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERFPS_RANGE_X			(85.0f)
#define PLAYERFPS_RANGE_Y			(55.0f)
#define PLAYERFPS_MOVESPEED			(1.5f)

#define PLAYERFPS_SHOTPOS_L			(D3DXVECTOR3(-10.0f, 0.0f, 5.0f))
#define PLAYERFPS_SHOTPOS_R			(D3DXVECTOR3( 10.0f, 0.0f, 5.0f))
#define PLAYERFPS_BULLETSPEED		(40.0f)
/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static const float targetSiteOffset[] = {
	-50.0f,
	0.0f,
	50.0f
};

/**************************************
プロトタイプ宣言
***************************************/


/**************************************
更新処理
***************************************/
void UpdatePlayerModelFPS(PLAYERMODEL *player)
{
	int x = GetHorizontalInputPress();
	int y = GetVerticalInputPress();

	//移動処理
	D3DXVECTOR3 moveDir = D3DXVECTOR3((float)x, (float)y, 0.0f);
	moveDir = *D3DXVec3Normalize(&moveDir, &moveDir) * PLAYERFPS_MOVESPEED;
	player->pos = player->pos + moveDir;

	player->destRot.z = x * PLAYER_DESTROT_MAX;

	player->pos.x = Clampf(-PLAYERFPS_RANGE_X, PLAYERFPS_RANGE_X, player->pos.x);
	player->pos.y = Clampf(-PLAYERFPS_RANGE_Y, PLAYERFPS_RANGE_Y, player->pos.y);

	//ロックオンターゲットの更新確認
	for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
	{
		if (!player->target[i].use)
		{
			continue;
		}

		if (!*player->target[i].active)
		{
			//player->target[i].use = false;
			//player->target[i].pos = NULL;
			ReleaseRockonTarget(player, i);
		}
	}

	//ターゲットサイト移動処理
	TARGETSITE *site = GetTargetSiteAdr(player->id);
	site->pos = player->pos;;

	//ロックオンサイトセット処理
	for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
	{
		if (player->target[i].use)
		{
			SetRockonSitePos(player->id * PLAYER_ROCKON_MAX + i, *player->target[i].pos);
		}
	}

	//ロックオンGUIセット処理
	SetLockonGUIPos(player->id, player->pos + D3DXVECTOR3(0.0f, -10.0f, 0.0f));

	//攻撃処理
	player->atkInterbal++;
	if (GetKeyboardTrigger(DIK_Z))
	{
		AttackPlayerModelFPS(player);
	}

	//ショット発射処理
	player->cntFrame++;
	if (player->cntFrame % PLAYER_SHOT_INTERBAL == 0)
	{
		SetPlayerBullet(player->pos + PLAYERFPS_SHOTPOS_L, PLAYERFPS_BULLETSPEED);
		SetPlayerBullet(player->pos + PLAYERFPS_SHOTPOS_R, PLAYERFPS_BULLETSPEED);
	}
}

/**************************************
入場処理
***************************************/
void EnterPlayerModelFPS(PLAYERMODEL *player)
{
	player->cntFrame = 0;
	player->atkInterbal = PLAYER_HOMINGATK_INTERBAL;

	//ターゲットサイト設定処理
	TARGETSITE *site = GetTargetSiteAdr(player->id);
	site->active = true;
	site->pos = site->targetPos = player->pos;// + D3DXVECTOR3(0.0f, 0.0f, PLAYERFPS_TARGETSITE_POS_Z);

	//ロックオン対象初期化
	for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
	{
		player->target[i].use = false;
	}
	player->lockonNum = 0;

	//ロックオンGUI表示
	GetLockonGUIAdr(player->id)->active = true;
}

/**************************************
退場処理
***************************************/
void ExitPlayerModelFPS(PLAYERMODEL *player)
{

}


/**************************************
攻撃処理
***************************************/
void AttackPlayerModelFPS(PLAYERMODEL *player)
{
	if (player->atkInterbal < PLAYER_HOMINGATK_INTERBAL)
	{
		return;
	}

	for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
	{
		if (player->target[i].pos == NULL)
		{
			continue;
		}

		for (int j = 0; j < 4; j++)
		{
			SetPlayerMissile(player->target[i].pos, player->target[i].hp, player->target[i].active, player->pos);
		}
		ReleaseRockonTarget(player, i);
		player->target[i].use = false;
	}
	player->atkInterbal = 0;
	player->lockonNum = 0;
}