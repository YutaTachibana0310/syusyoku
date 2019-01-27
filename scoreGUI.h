//=============================================================================
//
// スコアGUIヘッダ [ScoreGUI.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SCOREGUI_H_
#define _SCOREGUI_H_

#include "main.h"
#include <stdio.h>
// マクロ定義


//*****************************************************************************
// 構造体定義
//*****************************************************************************
typedef struct {
	float					radius;						// ポリゴンの半径
	float					angle;						// ポリゴンの角度
}SCOREGUI;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScoreGUI(int num);							//スコアGUI初期化処理
void UninitScoreGUI(int num);							//スコアGUI終了処理
void UpdateScoreGUI(void);								//スコアGUI更新処理
void DrawScoreGUI(void);								//スコアGUI描画処理
void DrawScoreGUIDebug(void);
void SaveSettingScoreGUI(FILE *fp);
void LoadSettingsScoreGUI(FILE *fp);
#endif
