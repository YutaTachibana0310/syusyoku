//==========================================
//
//雲ヘッダ[cloud.h]
//Author:GP11A341 21 立花雄太
//
//==========================================
#ifndef _CLOUD_H_
#define _CLOUD_H_

#include "main.h"

/******************************************
マクロ定義
*******************************************/

/******************************************
構造体定義
*******************************************/
typedef struct _CLOUD
{
	bool active;			//使用フラグ
	D3DXVECTOR3 pos;		//座標
	float rot;				//回転量
	_CLOUD* next;			//リスト構造の次の雲へのポインタ
}CLOUD;

/******************************************
プロトタイプ宣言
*******************************************/
void InitCloud(int num);
void UninitCloud(void);
void UpdateCloud(void);
void DrawCloud(void);
void SetCloud(D3DXVECTOR3 pos);
#endif