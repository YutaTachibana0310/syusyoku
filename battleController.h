//=====================================
//
//バトルコントローラヘッダ[battleController.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _BATTLECONTROLLER_H_
#define _BATTLECONTROLLER_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define BATTLE_SPACE_DIVIDE_NUM			(4)					//エネミー生成範囲分割数
#define BATTLE_SPACE_MAX (BATTLE_SPACE_DIVIDE_NUM*BATTLE_SPACE_DIVIDE_NUM)	//分割された空間の総数
#define BATTLE_FUZZY_NEAR_BORDER		(0.0f)				//距離に関するファジィ理論のしきい値1
#define BATTLE_FUZZY_MIDDLE_BORDER		(SCREEN_HEIGHT)		//距離に関するファジィ理論のしきい値2
#define BATTLE_FUZZY_FAR_BORDER			(SCREEN_WIDTH*2.5f)	//距離に関するファジィ理論のしきい値3
#define BATTLE_FUZZY_RECENTLY_BORDER	(120.0f)			//時間に関するファジィ理論のしきい値1
#define BATTLE_FUZZY_LATELY_BORDER		(1200.0f)			//時間に関するファジィ理論のしきい値2
#define BATTLE_CUBEEMMITT_NUM			(3)
#define BATTLE_CUBEEMMITT_SPEED			(10.0f)
#define BATTLE_EMMITT_INTERBAL			(60)

/**************************************
構造体定義
***************************************/
//バトルコントローラの各ステート
enum BATTLECONTROLLER_STATE {
	BattleNormalTime,
	BattleWaitBonusTimeBegin,
	BattleBonusTime,
	BattleBonusIntebal,
	BattleChangeView,
	BattleStateMax
};

//バトルコントローラーの視点モード
enum BATTLE_VIEWMODE {
	BattleViewFPS,
	BattleViewTop,
	BattleViewSide,
	BattleViewMax
};

//バトルコントローラー構造体
typedef struct {
	D3DXVECTOR2 enemyEmmittPos[BATTLE_SPACE_MAX];		//エネミー生成座標
	DWORD lastEmittFrame[BATTLE_SPACE_MAX];				//各空間にエネミーが生成された最後のフレーム
	DWORD cntFrame[BattleViewMax];						//フレームカウント
	D3DXVECTOR3 checkPos[BATTLE_SPACE_MAX];				//判定座標
	D3DXVECTOR3 emmittPos[BATTLE_SPACE_MAX];			//エネミー生成位置
	DWORD bonusStartFrame;								//ボーナスタイム開始タイミング

	int currentState;									//現在の状態
	int prevState;										//一つ前の状態

	int viewMode;										//視点モード
	int nextViewMode;									//次に遷移する視点モード
}BATTLECONTROLLER;


/**************************************
プロトタイプ宣言
***************************************/
void InitBattleController(int num);
void UninitBattleController(int num);
void UpdateBattleController(void);
bool IsBonusTime(void);
void EmmittFromFuzzy(BATTLECONTROLLER *controller);
void ChangeStateBattleController(int next);
bool CheckEscapedBonusCube(void);
void ChangeViewModeBattleController(int next);
int GetBattleViewMode(void);
void SetBonusTimePresen(void);
#endif