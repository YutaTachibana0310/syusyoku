//=============================================================================
//
// タイトルシーンヘッダ [TitleScene.h]
// Author : GP11A341 21 立花雄太
//
//=============================================================================
#ifndef _TITLESCENE_H_
#define _TITLESCENE_H_

#include "main.h"
#include "sceneManager.h"

// マクロ定義

//*****************************************************************************
// 構造体定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitTitleScene(int num);		//タイトルシーン初期化処理
void UninitTitleScene(int num);			//タイトルシーン終了処理
void UpdateTitleScene(void);			//タイトルシーン更新処理
void DrawTitleScene(void);				//タイトルシーン描画処理
void DrawTitleLogo(void);				//タイトルロゴ描画
#endif
