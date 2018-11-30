//=====================================
//
//GUIマネージャヘッダ[GUIManager.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
enum GUI_NUMTEXTURE
{
	GUI_NUMSCORE,
	GUI_NUMLOCKON,
	GUI_NUMTEX_MAX
};
/**************************************
プロトタイプ宣言
***************************************/
void InitGUIManager(int num);
void UninitGUIManager(int num);
void UpdateGUIManager(void);
void DrawGUIManager(void);
void DrawGUINum(GUI_NUMTEXTURE textureID, int num, VERTEX_2D *vtxWk);
#endif