//=====================================
//
//ロックオンサイトヘッダ[rockonSite.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _ROCKONSITE_H_
#define _ROCKONSITE_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;
	int id;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 screenPos;
	float scale;
	int cntFrame;
	float alpha;
	float rot;
}ROCKONSITE;

/**************************************
プロトタイプ宣言
***************************************/
void InitRockonSite(int num);
void UninitRockonSite(int num);
void UpdateRockonSite(void);
void DrawRockonSite(void);
ROCKONSITE *SetRockonSite(int id, D3DXVECTOR3 *target, bool *targetActive);
void ReleaseRockonSite(ROCKONSITE *ptr);
void SetRockonSitePos(int id, D3DXVECTOR3 pos);
void ActivateRockonSite(int id);
#endif