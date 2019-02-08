//=============================================================================
//
// スコアGUIヘッダ [ScoreGUI.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _SCOREGUI_H_
#define _SCOREGUI_H_

#include "main.h"

// マクロ定義

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitScoreGUI(int num);							//スコアGUI初期化処理
void UninitScoreGUI(int num);							//スコアGUI終了処理
void UpdateScoreGUI(void);								//スコアGUI更新処理
void DrawScoreGUI(void);								//スコアGUI描画処理
#endif
