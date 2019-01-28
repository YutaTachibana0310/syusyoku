//=====================================
//
//ボーナスタイム更新処理[battleBonusTime.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "battleController.h"
#include "bonusTelop.h"
#include "bgmManager.h"
#include "enemyManager.h"

/**************************************
マクロ定義
***************************************/
#define BATTLEBONUS_BGM_FADE_DURATION		(120)
#define BATTLE_BONUS_DURATION			(570)				//ボーナスタイム時間		
#define BATTLE_BONUS_WAIT				(120)				//ボーナスタイムのスタートオフセット
#define BATTLE_BONUS_EMMITT_RANGE		(200.0f)			//ボーナスタイムのキューブ生成範囲
#define BATTLE_BONUS_POS_Z				(6000.0f)			//ボーナスタイム時のキューブ生成位置（Z）
#define BATTLE_BONUS_SPEED				(35.0f)				//ボーナスタイムのキューブスピード

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
入場処理
***************************************/
void OnEnterBattleBonusTime(BATTLECONTROLLER *controller)
{
	//テロップ再生
	StartBonusTelopAnim(true);

	//開始タイミングを保存
	controller->bonusStartFrame = controller->cntFrame;

	//BGM切り替え
	FadeInBGM(BGM_BONUSTIME, BATTLEBONUS_BGM_FADE_DURATION);
	FadeOutBGM(BGM_BATTLESCENE, BATTLEBONUS_BGM_FADE_DURATION);
}

/**************************************
更新処理
***************************************/
void OnUpdateBattleBonusTime(BATTLECONTROLLER *controller)
{
	controller->cntFrame++;
	//ボーナスタイム中にキューブを放出する期間
	if ((int)(controller->cntFrame - controller->bonusStartFrame) < BATTLE_BONUS_DURATION)
	{
		for (int i = 0; i < 10; i++)
		{
			D3DXVECTOR3 emmittPos;
			emmittPos.x = RandomRangef(-BATTLE_BONUS_EMMITT_RANGE, BATTLE_BONUS_EMMITT_RANGE);
			emmittPos.y = RandomRangef(-BATTLE_BONUS_EMMITT_RANGE, BATTLE_BONUS_EMMITT_RANGE);
			emmittPos.z = BATTLE_BONUS_POS_Z;

			EmmittCubeObject(1, &emmittPos, BATTLE_BONUS_SPEED);
		}
	}

	//ボーナスタイム終了判定
	if ((int)(controller->cntFrame - controller->bonusStartFrame) > BATTLE_BONUS_DURATION + BATTLE_BONUS_WAIT)
	{
		controller->cntFrame = controller->bonusStartFrame;
		FadeOutBGM(BGM_BONUSTIME, BATTLEBONUS_BGM_FADE_DURATION);
		FadeInBGM(BGM_BATTLESCENE, BATTLEBONUS_BGM_FADE_DURATION, true);
		ChangeStateBattleController(BattleNormalTime);
	}
}
