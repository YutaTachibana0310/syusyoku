//=====================================
//
//カメラシェイカー処理[cameraShaker.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "cameraShaker.h"

/**************************************
マクロ定義
***************************************/
#define CAMERASHAKER_DURATION		(30)

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static int cntFrame;
static D3DXVECTOR3 cameraOffset;
static float offsetLength;
static bool active;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitCameraShaker(int num)
{
	cntFrame = 0;
	cameraOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	offsetLength = 0.0f;
	active = false;
}

/**************************************
終了処理
***************************************/
void UninitCameraShaker(int num)
{

}

/**************************************
更新処理
***************************************/
void UpdateCameraShaker(void)
{
	if (!active)
		return;

	cntFrame++;

	if (cntFrame > CAMERASHAKER_DURATION)
	{
		cameraOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		active = false;
		return;
	}

	cameraOffset = D3DXVECTOR3(RandomRangef(-offsetLength, offsetLength), RandomRangef(-offsetLength, offsetLength), RandomRangef(-offsetLength, offsetLength));
}

/**************************************
セット処理
***************************************/
void SetCameraShaker(float length)
{
	cntFrame = 0;
	offsetLength = length;
	active = true;
}

/**************************************
オフセット値取得処理
***************************************/
D3DXVECTOR3 GetCameraOffsetValue(void)
{
	return cameraOffset;
}