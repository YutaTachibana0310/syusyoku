//=====================================
//
//プレイヤー遷移処理[playerTransition.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "playerModelTransition.h"
#include "battleCamera.h"
#include "targetSite.h"
#include "lockonGUI.h"

/**************************************
マクロ定義
***************************************/

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
void EnterPlayerModelTransition(PLAYERMODEL *player)
{
	player->cntFrame = 0;
	GetTargetSiteAdr(player->id)->active = false;
	GetLockonGUIAdr(player->id)->active = false;
	player->initPos = player->pos;
}

/**************************************
更新処理
***************************************/
void UpdatePlayerModelTransition(PLAYERMODEL *player)
{
	player->cntFrame++;
	player->pos = EaseOutCubicVector((float)player->cntFrame / BATTLECAMERA_MOVEFRAME, player->initPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (player->cntFrame == BATTLECAMERA_MOVEFRAME)
	{
		ChangeStatePlayerModel(player->nextState);
	}

}

/**************************************
退場処理
***************************************/
void ExitPlayerModelTransition(PLAYERMODEL *player)
{

}