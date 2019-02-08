//=====================================
//
//プレイヤーモデル一人称処理[playerModelSideView.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModelSideView.h"
#include "playerBullet.h"
#include "input.h"
#include "targetSite.h"
#include "dataContainer.h"
#include "lockonGUI.h"
#include "soundEffectManager.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERSIDE_BULLETSPEED			(20.0f)
#define PLAYERSIDE_RANGE_Y				(90.0f)
#define PLAYERSIDE_RANGE_Z				(170.0f)
#define PLAYERSIDE_MOVESPEED			(3.5f)

#define PLAYERSIDE_SHOTPOS_L			(D3DXVECTOR3(-10.0f, 0.0f, 5.0f))
#define PLAYERSIDE_SHOTPOS_R			(D3DXVECTOR3(10.0f, 0.0f, 5.0f))
#define PLAYERSIDE_TARGETSITE_OFFSET	(D3DXVECTOR3(0.0f, 0.0, 200.0f))

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
void ClampPlayerPositionSide(PLAYERMODEL *player);

/**************************************
更新処理
***************************************/
void UpdatePlayerModelSideView(PLAYERMODEL *player)
{
	int x = GetHorizontalInputPress();
	int y = GetVerticalInputPress();

	//移動処理
	D3DXVECTOR3 moveDir = D3DXVECTOR3(0.0f, (float)y, (float)x);
	D3DXVec3Normalize(&moveDir, &moveDir);
	player->pos += moveDir * PLAYERSIDE_MOVESPEED;

	//player->pos.y = Clampf(-PLAYERSIDE_RANGE_Y, PLAYERSIDE_RANGE_Y, player->pos.y);
	ClampPlayerPositionSide(player);
	player->pos.z = Clampf(-PLAYERSIDE_RANGE_Z, PLAYERSIDE_RANGE_Z, player->pos.z);

	//ロックオンターゲットの更新確認
	for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
	{
		if (!player->target[i].use)
		{
			continue;
		}

		if (!*player->target[i].active)
		{
			ReleaseRockonTarget(player, i);
		}
	}

	//ターゲットサイト移動処理
	//TARGETSITE *site = GetTargetSiteAdr(player->id);
	//site->pos = player->pos + PLAYERSIDE_TARGETSITE_OFFSET;
	SetTargetSitePosition(player->pos, player->id, D3DXVECTOR3(800.0f, 0.0f, 0.0f));
	UpdateTargetSite(GetAttackButtonPress());

	//ロックオンサイトセット処理
	int lockMax = GetLockonMax();
	for (int i = 0; i < lockMax; i++)
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
	if (player->atkInterbal == PLAYER_HOMINGATK_INTERBAL)
		PlaySE(SOUND_READY);

	if (GetAttackButtonRelease())
	{
		AttackPlayerMissile(player);
	}

	//ショットポジション更新
	D3DXVec3TransformCoord(&player->shotpos1, &PLAYERSIDE_SHOTPOS_L, &player->mtxWorld);
	D3DXVec3TransformCoord(&player->shotpos2, &PLAYERSIDE_SHOTPOS_R, &player->mtxWorld);

	//ショット発射処理
	player->cntFrame++;
	if (player->cntFrame % PLAYER_SHOT_INTERBAL == 0)// && !GetAttackButtonPress())
	{
		PlaySE(SOUND_SHOT);
		SetPlayerBullet(player->shotpos1, PLAYERSIDE_BULLETSPEED);
		SetPlayerBullet(player->shotpos2, PLAYERSIDE_BULLETSPEED);
	}
}

/**************************************
入場処理
***************************************/
void EnterPlayerModelSideView(PLAYERMODEL *player)
{
	player->cntFrame = 0;
	player->isInvincible = false;
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
void ExitPlayerModelSideView(PLAYERMODEL *player)
{
	
}

/**************************************
移動範囲制限処理
***************************************/
void ClampPlayerPositionSide(PLAYERMODEL *player)
{
	float posZ = player->pos.z + PLAYERSIDE_RANGE_Z;
	float border = posZ / (PLAYERSIDE_RANGE_Z * 2) * 40.0f + PLAYERSIDE_RANGE_Y;
	player->pos.y = Clampf(-border, border, player->pos.y);
}