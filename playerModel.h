//=====================================
//
//プレイヤーモデルヘッダ[playerModel.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PLAYERMODEL_H_
#define _PLAYERMODEL_H_

#include "main.h"
#include "EasingVector.h"
#include "battleCamera.h"

#define PLAYER_INITMOVE_FRAME	(30)
#define PLAYERMODEL_MAX			(4)
#define PLAYER_DESTROT_MAX		(0.51f)
#define PLAYER_MOVERANGE		(50.0f)
/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
enum PlayerState
{
	PlayerFPS,
	PlayerTopView,
	PlayerSideView,
	PlayerQuaterView,
	PlayerStateMax
};

typedef struct
{
	bool active;
	int id;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scale;

	D3DXVECTOR3 initPos;
	bool flgMove;
	int cntFrame;

	D3DXVECTOR3 destRot;

}PLAYERMODEL;

/**************************************
プロトタイプ宣言
***************************************/
void InitPlayerModel(int num);
void UninitPlayerModel(int num);
void UpdatePlayerModel(void);
void DrawPlayerModel(void);
PLAYERMODEL *GetPlayerAdr(int num);
void ChangeStatePlayerModel(int next);

#endif