//=====================================
//
//ランキングネームヘッダ[rankingName.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _RANKINGNAME_H_
#define _RANKINGNAME_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
enum DEFINE_CHARCHIP
{
	CHARCHIP_A,
	CHARCHIP_B,
	CHARCHIP_C,
	CHARCHIP_D,
	CHARCHIP_E,
	CHARCHIP_F,
	CHARCHIP_G,
	CHARCHIP_H,
	CHARCHIP_I,
	CHARCHIP_J,
	CHARCHIP_K,
	CHARCHIP_L,
	CHARCHIP_M,
	CHARCHIP_N,
	CHARCHIP_O,
	CHARCHIP_P,
	CHARCHIP_Q,
	CHARCHIP_R,
	CHARCHIP_S,
	CHARCHIP_T,
	CHARCHIP_U,
	CHARCHIP_V,
	CHARCHIP_W,
	CHARCHIP_X,
	CHARCHIP_Y,
	CHARCHIP_Z,
	CHARCHIP_ASTERISK,
	CHARCHIP_EXCLAMATION,
	CHARCHIP_QUESTION,
	CHARCHIP_END,
	CHARCHIP_MAX,
	CHARCHIP_NONE
};

/**************************************
プロトタイプ宣言
***************************************/
void InitRankingName(int num);
void UninitRankingName(int num);
void UpdateRankingName(void);
void DrawRankingName(void);

#endif