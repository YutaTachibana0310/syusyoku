//=============================================================================
//
// イージングヘッダ [Easing.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _EASING_H_
#define _EASING_H_

#include "d3dx9.h"

//*****************************************************************************
// 列挙体定義
//*****************************************************************************
enum EASINGVECTOR_TYPE
{
	InCubic,
	OutCubic,
	InOutCubic,
	Linear,
	InExponential,
	OutExponential,
	InOutExponential,
	EasingMax
};

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
D3DXVECTOR3 GetEasingValue(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal, EASINGVECTOR_TYPE type);
D3DXVECTOR3 EaseInCubic(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);			//InCubic計算処理
D3DXVECTOR3 EaseOutCubic(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);		//OutCubic計算処理
D3DXVECTOR3 EaseInOutCubic(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);		//InOutCubic計算処理
D3DXVECTOR3 EaseLinear(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);			//Linear計算処理
D3DXVECTOR3 EaseInExponential(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);	//InExponential計算処理
D3DXVECTOR3 EaseOutExponential(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);	//OutExponential計算処理
D3DXVECTOR3 EaseInOutExponential(float time, D3DXVECTOR3 start, D3DXVECTOR3 goal);//InOutExponential計算処理
#endif