//=====================================
//
//カメラシェイカーヘッダ[cameraShaker.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _CAMERASHAKER_H_
#define _CAMERASHAKER_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/

/**************************************
プロトタイプ宣言
***************************************/
void InitCameraShaker(int num);
void UninitCameraShaker(int num);
void UpdateCameraShaker(void);

void SetCameraShaker(float length);
D3DXVECTOR3 GetCameraOffsetValue(void);

#endif