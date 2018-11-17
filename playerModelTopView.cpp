//=====================================
//
//プレイヤーモデル一人称処理[playerModelTopView.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModelTopView.h"
#include "playerBullet.h"
#include "input.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERTOP_BULLETSPEED	(5.0f)
#define PLAYERTOP_RANGE_X		(80.0f)
#define PLAYERTOP_RANGE_Z		(70.0f)

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
	if (player->flgMove)
	{
		player->cntFrame++;
		player->pos = EaseInOutCubic((float)player->cntFrame / BATTLECAMERA_MOVEFRAME, player->initPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		if (player->cntFrame == BATTLECAMERA_MOVEFRAME)
		{
			player->flgMove = false;
		}
	}
	else
	{
		if (GetKeyboardPress(DIK_LEFT))
		{
			player->pos.x -= 1.0f;
			player->destRot.z = -PLAYER_DESTROT_MAX;
		}
		else if (GetKeyboardPress(DIK_RIGHT))
		{
			player->pos.x += 1.0f;
			player->destRot.z = PLAYER_DESTROT_MAX;
		}
		else
		{
			player->destRot.z = 0.0f;
		}

		if (GetKeyboardPress(DIK_UP))
		{
			player->pos.z += 1.0f;
		}
		else if (GetKeyboardPress(DIK_DOWN))
		{
			player->pos.z -= 1.0f;
		}
	}

	SetPlayerBullet(player->pos, PLAYERTOP_BULLETSPEED);

	player->pos.x = Clampf(-PLAYERTOP_RANGE_X, PLAYERTOP_RANGE_X, player->pos.x);
	player->pos.z = Clampf(-PLAYERTOP_RANGE_Z, PLAYERTOP_RANGE_Z, player->pos.z);
}

/**************************************
入場処理
***************************************/
void EnterPlayerModelTopView(PLAYERMODEL *player)
{
	player->cntFrame = 0;
	player->flgMove = true;
	player->initPos = player->pos;
}
