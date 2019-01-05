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
	D3DXVECTOR3 rotValue;		//1フレームあたりの回転量
	float moveSpeed;			//1フレームあたりの移動量
	COLLIDER_CUBE collider;		//当たり判定
	bool active;
	float hp;

}CUBE_OBJECT;

/**************************************
プロトタイプ宣言
***************************************/
void InitCubeObject(int num);
void UninitCubeObject(int num);
void UpdateCubeObject(void);
void DrawCubeObject(void);

void CollisionCubeObjectAndBullet(void);
void LockonCubeObject(void);

#endif