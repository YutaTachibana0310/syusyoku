//=====================================
//
//プレイヤーモデル一人称処理[playerModelTopView.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModelTopView.h"
#include "playerBullet.h"
#include "input.h"
#include "targetSite.h"
#include "playerMissile.h"
#include "lockonGUI.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERTOP_MOVESPEED		(1.5f)
#define PLAYERTOP_RANGE_X		(80.0f)
#define PLAYERTOP_RANGE_Z		(70.0f)

#define PLAYERTOP_SHOTPOS_L		(D3DXVECTOR3(-10.0f, 0.0f, 5.0f))
#define PLAYERTOP_SHOTPOS_R		(D3DXVECTOR3( 10.0f, 0.0f, 5.0f))
#define PLAYERTOP_BULLETSPEED	(40.0f)
#define PLAYERTOP_SITEPOS		(D3DXVECTOR3(0.0f, 0.0f, 250.0f))

/**************************************
構造体定義
***************************************

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/


/**************************************
更新処理
***************************************/
void UpdatePlayerModelTopView(PLAYERMODEL *player)
{
	int x = GetHorizontalInputPress();
	int y = GetVerticalInputPress();

	//移動処理
	D3DXVECTOR3 moveDir = D3DXVECTOR3((float)x, 0.0f, (float)y);
	D3DXVec3Normalize(&moveDir, &moveDir);
	player->pos += moveDir * PLAYERTOP_MOVESPEED;

	player->destRot.z = x * PLAYER_DESTROT_MAX;

	player->pos.x = Clampf(-PLAYERTOP_RANGE_X, PLAYERTOP_RANGE_X, player->pos.x);
	player->pos.z = Clampf(-PLAYERTOP_RANGE_Z, PLAYERTOP_RANGE_Z, player->pos.z);

	//ターゲットの更新確認
	TARGETSITE *site = GetTargetSiteAdr(player->id);
	site->pos = player->pos + PLAYERTOP_SITEPOS;

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
		AttackPlayerMissile(player);
	}

	//ショットポジション更新
	D3DXVec3TransformCoord(&player->shotpos1, &PLAYERTOP_SHOTPOS_L, &player->mtxWorld);
	D3DXVec3TransformCoord(&player->shotpos2, &PLAYERTOP_SHOTPOS_R, &player->mtxWorld);

	//ショット発射処理
	player->cntFrame++;
	if (player->cntFrame % PLAYER_SHOT_INTERBAL == 0)
	{
		SetPlayerBullet(player->shotpos1, PLAYERTOP_BULLETSPEED);
		SetPlayerBullet(player->shotpos2, PLAYERTOP_BULLETSPEED);
	}
}

/**************************************
入場処理
***************************************/
void EnterPlayerModelTopView(PLAYERMODEL *player)
{
	player->cntFrame = 0;
	player->atkInterbal = PLAYER_HOMINGATK_INTERBAL;

	//ターゲットサイト設定処理
	TARGETSITE *site = GetTargetSiteAdr(player->id);
	site->active = true;
	site->pos = player->pos;

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
void ExitPlayerModelTopView(PLAYERMODEL *player)
{

}