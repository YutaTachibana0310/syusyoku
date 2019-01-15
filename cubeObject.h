//=====================================
//
//キューブオブジェクトヘッダ[cubeObject.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _CUBEOBJECT_H_
#define _CUBEOBJECT_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;				//アクティブ判定
	WORD id;					//固有ID
	D3DXVECTOR3 rotValue;		//1フレームあたりの回転量
	float moveSpeed;			//1フレームあたりの移動量
	COLLIDER_CUBE collider;		//当たり判定
	float hp;					//HP
	D3DXVECTOR3 pos;			//座標
	D3DXVECTOR3 rot;			//回転
	float scale;				//スケール
}CUBE_OBJECT;

/**************************************
プロトタイプ宣言
***************************************/
void InitCubeObject(int num);
void UninitCubeObject(int num);
void UpdateCubeObject(void);
void DrawCubeObject(void);

void LockonCubeObject(void);
void DamageAllCubeObject(void);
bool SetCubeObject(D3DXVECTOR3 *setPos, float speed);
#endif