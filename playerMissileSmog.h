//=====================================
//
//プレイヤーミサイルスモッグヘッダ[PlayerMissileSmog.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _PLAYERMISSILESMOG_H_
#define _PLAYERMISSILESMOG_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;
	D3DXVECTOR3 pos;

	int cntFrame;
	int patternAnim;
}PLAYERMISSILESMOG;

/**************************************
プロトタイプ宣言
***************************************/
void InitPlayerMissileSmog(int num);
void UninitPlayerMissileSmog(int num);
void UpdatePlayerMissileSmog(void);
void DrawPlayerMissileSmog(void);
void SetPlayerMissileSmog(D3DXVECTOR3 pos);
#endif