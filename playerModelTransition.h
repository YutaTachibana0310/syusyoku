//=====================================
//
//プレイヤー遷移ヘッダ[playerTransition.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PLAYERTRANSITION_H_
#define _PLAYERTRANSITION_H_

#include "main.h"
#include "playerModel.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
void UpdatePlayerModelTransition(PLAYERMODEL *player);
void EnterPlayerModelTransition(PLAYERMODEL *player);
void ExitPlayerModelTransition(PLAYERMODEL *player);
#endif