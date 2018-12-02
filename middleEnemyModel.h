//=====================================
//
//ミドルエネミーモデルヘッダ[middleEnemyModel.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _MIDDLEENMYMODEL_H_
#define _MIDDLEENMYMODEL_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define MIDDLEENEMY_MAX			(36)
/**************************************
構造体定義
***************************************/
typedef struct
{
	bool active;

	D3DXVECTOR3 pos;
	
}MIDDLEENMYMODEL;
/**************************************
プロトタイプ宣言
***************************************/
void InitMiddleEnemyModel(int num);
void UninitMiddleEnemyModel(int num);
void UpdateMiddleEnemyModel(void);
void DrawMiddleEnemyModel(void);

#endif