//=====================================
//
//チュートリアルコントローラーヘッダ[tutorialController.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _TUTORIALCONTROLLER_H_
#define _TUTORIALCONTROLLER_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/

/**************************************
構造体定義
***************************************/
enum TUTORIAL_INDEX {
	TutorialMove,
	TutorialLockon,
	TutorialHomingAttack,
	TutorialHardCube,
	TutorialBonusCube,
	TutorialBonusTime,
	TutorialEnd,
	TutorialIndexMax
};
/**************************************
プロトタイプ宣言
***************************************/
void InitTutorialController(int num);
void UninitTutorialController(int num);
void UpdateTutorialController(void);
void DrawTutorialController(void);
void ChangeStateTutorialController(TUTORIAL_INDEX next);

//入場処理
void OnEnterTutorialMove(void);
void OnEnterTutorialLockon(void);
void OnEnterTutorialHomingAttack(void);
void OnEnterTutorialHardCube(void);
void OnEnterTutorialBonusCube(void);
void OnEnterTutorialBonusTime(void);
void OnEnterTutorialEnd(void);

//更新処理
void OnUpdateTutorialMove(void);
void OnUpdateTutorialLockon(void);
void OnUpdateTutorialHomingAttack(void);
void OnUpdateTutorialHardCube(void);
void OnUpdateTutorialBonusCube(void);
void OnUpdateTutorialBonusTime(void);
void OnUpdateTutorialEnd(void);

//描画処理
void OnDrawTutorialMove(LPDIRECT3DTEXTURE9 tex);
void OnDrawTutorialLockon(LPDIRECT3DTEXTURE9 tex);
void OnDrawTutorialHomingAttack(LPDIRECT3DTEXTURE9 tex);
void OnDrawTutorialHardCube(LPDIRECT3DTEXTURE9 tex);
void OnDrawTutorialBonusCube(LPDIRECT3DTEXTURE9 tex);
void OnDrawTutorialBonusTime(LPDIRECT3DTEXTURE9 tex);
void OnDrawTutorialEnd(LPDIRECT3DTEXTURE9 tex);
#endif