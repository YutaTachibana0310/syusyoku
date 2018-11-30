//=====================================
//
//プレイヤーモデル一人称処理[playerModelSideView.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModelSideView.h"
#include "playerBullet.h"
#include "input.h"

/**************************************
マクロ定義
***************************************/
#define PLAYERSIDE_BULLETSPEED		(10.0f)
#define PLAYERSIDE_RANGE_Y			(75.0f)
#define PLAYEYSIDE_RANGE_Z			(110.0f)

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
更新処理
***************************************/
void UpdatePlayerModelSideView(PLAYERMODEL *player)
{
	if (GetKeyboardPress(DIK_LEFT))
	{
		player->pos.z -= 1.0f;
	}
	else if (GetKeyboardPress(DIK_RIGHT))
	{
		player->pos.z += 1.0f;
	}

	if (GetKeyboardPress(DIK_UP))
	{
		player->pos.y += 1.0f;
		player->destRot.z = -PLAYER_DESTROT_MAX;
	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		player->pos.y -= 1.0f;
		player->destRot.z = PLAYER_DESTROT_MAX;
	}
	else
	{
		player->destRot.z = 0.0f;
	}

	SetPlayerBullet(player->pos, PLAYERSIDE_BULLETSPEED);

	player->pos.y = Clampf(-PLAYERSIDE_RANGE_Y, PLAYERSIDE_RANGE_Y, player->pos.y);
	player->pos.z = Clampf(-PLAYEYSIDE_RANGE_Z, PLAYEYSIDE_RANGE_Z, player->pos.z);
}

/**************************************
入場処理
***************************************/
void EnterPlayerModelSideView(PLAYERMODEL *player)
{
	player->cntFrame = 0;
	player->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	player->initPos = player->pos;
}

/**************************************
退場処理
***************************************/
void ExitPlayerModelSideView(PLAYERMODEL *player)
{
	
}