//=====================================
//
//ターゲットサイトヘッダ[targetSite.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _TARGETSITE_H_
#define _TARGETSITE_H_

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
	D3DXVECTOR3 pos;
	D3DXVECTOR3 targetPos;
	D3DXVECTOR3 insideRot, outsideRot;

	//D3DXVECTOR3 topL, topR, bottomL, bottomR;

}TARGETSITE;
/**************************************
プロトタイプ宣言
***************************************/
void InitTargetSite(int num);
void UninitTargetSite(int num);
void UpdateTargetSite(void);
void DrawTargetSite(void);
void SetTargetSitePosition(D3DXVECTOR3 pos, int id);
TARGETSITE *GetTargetSiteAdr(int id);
bool CollisionTargetSite(int id, const D3DXVECTOR3* pos, D3DXVECTOR3* siteScreenPos);
void RockonEnemy(int id);
#endif