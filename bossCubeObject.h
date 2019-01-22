//=====================================
//
//ボスキューブオブジェクトヘッダ[bossCubeObject.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BOSSCUBEOBJECT_H_
#define _BOSSCUBEOBJECT_H_

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
	D3DXVECTOR3 rot;
	D3DXVECTOR3 scale;

}BOSS_CUBE_OBJECT;
/**************************************
プロトタイプ宣言
***************************************/
void InitBossCubeObject(int num);
void UninitBossCubeObject(int num);
void UpdateBossCubeObject(void);
void DrawBossCubeObject(void);

#endif