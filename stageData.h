//=====================================
//
//ステージデータヘッダ[stageData.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _STAGEDATA_H_
#define _STAGEDATA_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct
{
	int emmittFrame;
	int type;
	D3DXVECTOR3 initPos;
	D3DXVECTOR3 targetPos;
	D3DXVECTOR3 controller1;
	D3DXVECTOR3 controller2;
}STAGE_DATA;
/**************************************
プロトタイプ宣言
***************************************/
void InitStageData(int num);
void UninitStageData(int num);
int UpdateStageData(STAGE_DATA **out, DWORD currentFrame, int mode);
bool IsStageEnd(void);
void SetBonusPresenFlag(bool state);
void SetStageDataContainerHead(int n);
#endif