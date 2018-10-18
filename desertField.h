//=========================================================
//
//砂漠フィールドヘッダ［desertField.h］
//Author：GP11A341 21 立花雄太
//=========================================================
#ifndef _DESERTFIELD_H_
#define _DESERTFIELD_H_

#include "main.h"

/*********************************************************
構造体定義
*********************************************************/
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 vtxBuff;	//頂点バッファ
	D3DXVECTOR3 pos;					//フィールドのワールド座標
	float *heightMap;					//頂点の高さのマップ
}DESERTFIELD;
/************************************************************
プロトタイプ宣言
*************************************************************/
void InitDesertField(int num);
void UninitDesertField(void);
void UpdateDesertField(void);
void DrawDesertField(void);


#endif