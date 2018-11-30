//=====================================
//
//ロックオンGUIヘッダ[lockonGUI.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _LOCKONGUI_H_
#define _LOCKONGUI_H_

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
	int state;
}LOCKONGUI;
/**************************************
プロトタイプ宣言
***************************************/
void InitLockonGUI(int num);
void UninitLockonGUI(int num);
void UpdateLockonGUI(void);
void DrawLockonGUI(void);
void SetLockonGUIPos(int id, D3DXVECTOR3 pos);
LOCKONGUI *GetLockonGUIAdr(int id);
#endif