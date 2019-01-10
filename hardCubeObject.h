//=====================================
//
//ハードキューブオブジェクトヘッダ[hardCubeObject.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _HARDCUBEOBJECT_H_
#define _HARDCUBEOBJECT_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct {
	bool active;				//アクティブ判定
	float moveSpeed;			//移動スピード
	D3DXVECTOR3 rotValue;		//回転量
	COLLIDER_CUBE collider;		//当たり判定
	float hp;					//HP
	D3DXVECTOR3 pos;			//座標
	D3DXVECTOR3 rot;			//回転量

}HARD_CUBE_OBJECT;
/**************************************
プロトタイプ宣言
***************************************/
void InitHardCubeObject(int num);
void UninitHardCubeObject(int num);
void UpdateHardCubeObject(void);
void DrawHardCubeObject(void);

void LockonHardCubeObject(void);

#endif