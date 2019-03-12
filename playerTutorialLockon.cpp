//=====================================
//
//プレイヤーモデル一人称処理[playerTutorialLockon.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModel.h"
#include "playerBullet.h"
#include "input.h"
#include "targetSite.h"
#include "lockonGUI.h"
#include "dataContainer.h"
#include "soundEffectManager.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERTUTOLOCKON_RANGE_X			(85.0f)
#define PLAYERTUTOLOCKON_RANGE_Y			(55.0f)
#define PLAYERTUTOLOCKON_MOVESPEED			(1.5f)

#define PLAYERTUTOLOCKON_SHOTPOS_L			(D3DXVECTOR3(-10.0f, 0.0f, 5.0f))
#define PLAYERTUTOLOCKON_SHOTPOS_R			(D3DXVECTOR3( 10.0f, 0.0f, 5.0f))
#define PLAYERTUTOLOCKON_BULLETSPEED		(40.0f)
/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static int lockonNum;

/**************************************
プロトタイプ宣言
***************************************/


/**************************************
更新処理
***************************************/
void UpdatePlayerModelTutorialLockon(PLAYERMODEL *player)
{
	int x = GetHorizontalInputPress();
	int y = GetVerticalInputPress();

	//移動処理
	D3DXVECTOR3 moveDir = D3DXVECTOR3((float)x, (float)y, 0.0f);
	moveDir = *D3DXVec3Normalize(&moveDir, &moveDir) * PLAYERTUTOLOCKON_MOVESPEED;
	player->pos = player->pos + moveDir;

	player->destRot.z = x * PLAYER_DESTROT_MAX;

	player->pos.x = Clampf(-PLAYERTUTOLOCKON_RANGE_X, PLAYERTUTOLOCKON_RANGE_X, player->pos.x);
	player->pos.y = Clampf(-PLAYERTUTOLOCKON_RANGE_Y, PLAYERTUTOLOCKON_RANGE_Y, player->pos.y);

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
	//TARGETSITE *site = GetTargetSiteAdr(player->id);
	//site->pos = player->pos;
	SetTargetSitePosition(player->pos, player->id, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	bool pressAtkButton = GetAttackButtonPress();
	UpdateTargetSite(pressAtkButton);
	if (GetAttackButtonTrigger())
		lockonNum++;
	

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

	if (GetAttackButtonRelease())
	{
		for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
		{
			if (player->target[i].pos == NULL)
				continue;

			ReleaseRockonTarget(player, i);
			player->target[i].use = false;
			player->lockonNum = 0;
		}
		player->atkInterbal = 0;
	}

	//ショットポジション更新
	D3DXVec3TransformCoord(&player->shotpos1, &PLAYERTUTOLOCKON_SHOTPOS_L, &player->mtxWorld);
	D3DXVec3TransformCoord(&player->shotpos2, &PLAYERTUTOLOCKON_SHOTPOS_R, &player->mtxWorld);

	//ショット発射処理
	player->cntFrame++;
	if (player->cntFrame % PLAYER_SHOT_INTERBAL == 0 && !GetAttackButtonPress())
	{
		SetPlayerBullet(player->shotpos1, PLAYERTUTOLOCKON_BULLETSPEED);
		SetPlayerBullet(player->shotpos2, PLAYERTUTOLOCKON_BULLETSPEED);
	}
}

/**************************************
入場処理
***************************************/
void EnterPlayerModelTutorialLockon(PLAYERMODEL *player)
{
	player->cntFrame = 0;
	player->atkInterbal = PLAYER_HOMINGATK_INTERBAL;

	//ターゲットサイト設定処理
	TARGETSITE *site = GetTargetSiteAdr(player->id);
	site->active = true;
	site->pos = site->targetPos = player->pos;// + D3DXVECTOR3(0.0f, 0.0f, PLAYERTUTOLOCKON_TARGETSITE_POS_Z);

	//ロックオン対象初期化
	for (int i = 0; i < PLAYER_ROCKON_MAX; i++)
	{
		player->target[i].use = false;
	}
	player->lockonNum = 0;

	//ロックオンGUI表示
	GetLockonGUIAdr(player->id)->active = true;

	lockonNum = 0;

}

/**************************************
退場処理
***************************************/
void ExitPlayerModelTutorialLockon(PLAYERMODEL *player)
{

}

/**************************************
チュートリアルロックオン回数取得処理
***************************************/
int GetTutorialLockOnNum(void)
{
	return lockonNum;
}