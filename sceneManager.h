//=====================================
//
//シーンマネージャヘッダ[sceneManager.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
enum DefineScene
{
	LogoScene = 0,
	TitleScene,
	BattleScene,
	ResultScene,
	TutorialScene,
	GamveoverScene,
	StageClearScene,
	NameEntryScene,
	DefineSceneMax
};

enum SceneResult
{
	SceneRunning,
	SceneFinished,
	SceneResultMax
};

/**************************************
プロトタイプ宣言
***************************************/
void InitSceneManager(int* ptr);
void UninitSceneManager(int num);
void UninitScene(int num);
void UpdateSceneManager(void);
void DrawSceneManager(void);
void SetScene(DefineScene sceneId);
int GetNextScene(void);
#endif