//=====================================
//
//ボーナスキューブヘッダ[bonusCube.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BONUSCUBE_H_
#define _BONUSCUBE_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct {
	bool active;						//アクティブ判定
	WORD id;							//固有ID
	float moveSpeed;					//移動スピード
	D3DXVECTOR3 rotValue;				//回転量
	COLLIDER_CUBE collider;				//当たり判定
	float hp;							//HP
	D3DXVECTOR3 pos;					//座標
	D3DXVECTOR3 rot;					//回転量
	float scale;						//スケール

	int cntFrame;						//カウントフレーム
	D3DXVECTOR3 startPos, goalPos;		//スタート座標、エンド座標
	int cntMove;						//移動回数
}BONUS_CUBE_OBJECT;

/**************************************
プロトタイプ宣言
***************************************/
void InitBonusCube(int num);
void UninitBonusCube(int num);
void UpdateBonusCube(void);
void DrawBonusCube(void);

void LockonBonusCube(void);
bool SetBonusCube(D3DXVECTOR3 *setPos);
bool IsAllBonusCubeDisable(void);

#endif