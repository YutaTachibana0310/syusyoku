//=============================================================================
//
// コライダーヘッダ [collider.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "main.h"
//*****************************************************************************
// 構造体定義
//*****************************************************************************

/*
四角形ポリゴンを表す構造体
*/
typedef struct
{
	D3DXVECTOR3 topL;		//左上頂点
	D3DXVECTOR3 bottomL;	//左下頂点
	D3DXVECTOR3 bottomR;	//右下頂点
	D3DXVECTOR3 topR;		//右上頂点
	D3DXVECTOR3 nor;		//法線ベクトル
}PLANE;

/*
三角形ポリゴンを表す構造体
*/
typedef struct
{
	D3DXVECTOR3 a;			//頂点1
	D3DXVECTOR3 b;			//頂点2
	D3DXVECTOR3 c;			//頂点3
	D3DXVECTOR3 nor;		//法線ベクトル
}TRIANGLE;

typedef struct
{
	D3DXVECTOR3 pos;		//中心位置
	float radius;			//半径
	bool active;			//アクティブ判定
}SPHERE;
//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
bool CheckHitTriangleAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, TRIANGLE tri, D3DXVECTOR3 *out);
bool CheckHitPlaneAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, PLANE plane, D3DXVECTOR3 *out);
bool ChechHitBoundingSphere(SPHERE s1, SPHERE s2);
void UpdateCollision(void);
#endif