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
#define PLAYERFPS_BULLETSPEED		(10.0f)
#define PLAYERFPS_RANGE_X			(85.0f)
#define PLAYERFPS_RANGE_Y			(55.0f)
#define PLAYERFPS_MOVESPEED			(1.5f)

#define PLAYERFPS_TARGETSITE_POS_Z	(600.0f)
#define PLAYERFPS_TARGETSITE_POS_XY	(50.0f)
#define PLAYERFPS_TARGETSITE_POS	(D3DXVECTOR3(0.0f, 0.0f, 0.0f))

#define PLAYERFPS_ATTACKINTERBAL	(60)

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

	//遷移直後の移動処理
	if (player->flgMove)
	{
		player->cntFrame++;
		player->pos = EaseOutCubic((float)player->cntFrame / BATTLECAMERA_MOVEFRAME, player->initPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		if (player->cntFrame == BATTLECAMERA_MOVEFRAME)
		{
			player->flgMove = false;
		}

		GetTargetSiteAdr(player->id)->targetPos = player->pos + D3DXVECTOR3(0.0f, 0.0f, PLAYERFPS_TARGETSITE_POS_Z);
	}
	//通常更新処理
	else
	{
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
				player->target[i].use = false;
				player->target[i].pos = NULL;
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
	}


}

/**************************************
入場処理
***************************************/
void EnterPlayerModelFPS(PLAYERMODEL *player)
{
	player->flgMove = true;
	player->cntFrame = 0;
	player->atkInterbal = PLAYERFPS_ATTACKINTERBAL;

	//ターゲットサイト設定処理
	TARGETSITE *site = GetTargetSiteAdr(player->id);
	site->active = true;
	site->pos = site->targetPos = player->pos;// + D3DXVECTOR3(0.0f, 0.0f, PLAYERFPS_TARGETSITE_POS_Z);

	//ロックオン対象初期化
	for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
	{
		player->target[i].use = false;
	}

	//ロックオンGUI表示
	GetLockonGUIAdr(player->id)->active = true;
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
		ReleaseRockonTarget(&player->target[i]);
		player->target[i].use = false;
	}
	player->atkInterbal = 0;
}