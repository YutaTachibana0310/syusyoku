//=====================================
//
//シーンフェードヘッダ[sceneFade.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SCENEFADE_H_
#define _SCENEFADE_H_

#include "main.h"
#include "sceneManager.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
enum SceneFadeState
{
	SceneFadeIn,
	SceneFadeWait,
	SceneFadeOut,
	SceneFadeNone,
	SceneFadeStateMax
};

/**************************************
プロトタイプ宣言
***************************************/
void InitSceneFade(int num);
void UninitSceneFade(int num);
void UpdateSceneFade(void);
void DrawSceneFade(void);

void ChangeStateSceneFade(SceneFadeState state);
void SetSceneFade(DefineScene next);
#endif