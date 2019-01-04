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

//スフィアコライダー
typedef struct
{
	D3DXVECTOR3 *pos;		//判定を持つオブジェクトの座標ポインタ
	D3DXVECTOR3 offset;		//対象オブジェクトからのオフセット
	float radius;			//半径
	bool active;			//アクティブ判定
}COLLIDER_SPHERE;

//キューブコライダー
typedef struct
{	
	D3DXVECTOR3 *pos;		//判定を持つオブジェクトの座標ポインタ
	D3DXVECTOR3 offset;		//対象オブジェクトからのオフセット
	D3DXVECTOR3 length;		//端点への距離
	bool active;			//アクティブ判定
}COLLIDER_CUBE;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitCollider(int num);
void UninitCollider(int num);

bool CheckHitTriangleAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, TRIANGLE tri, D3DXVECTOR3 *out);
bool CheckHitPlaneAndLine(D3DXVECTOR3 start, D3DXVECTOR3 end, PLANE plane, D3DXVECTOR3 *out);
bool CheckHitBoundingSphere(const COLLIDER_SPHERE *s1, const COLLIDER_SPHERE *s2);
bool ChechHitBoundingCube(const COLLIDER_CUBE *c1, const COLLIDER_CUBE *c2);

void DrawBoundingSphere(const COLLIDER_SPHERE *s);
void DrawBoundingCube(const COLLIDER_CUBE *cube);

#endif