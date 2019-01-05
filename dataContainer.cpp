//=====================================
//
//データコンテナ処理[dataContainer.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "dataContainer.h"
#include "powerUpTelop.h"

/**************************************
マクロ定義
***************************************/
#define DATACONTAINER_POWEUP_MAX		(7)		//パワーアップ最大回数

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
//スコア関連パラメータ
static int currentScore, highScore;

//パワーアップ関連パラメータ
static int cntPowerUp, shotLevel, lockLevel;

//パワーアップのしきい値
const int PowerUpBorder[DATACONTAINER_POWEUP_MAX] = {
	500000,
	1000000,
	1500000,
	2000000,
	2500000,
	3000000,
	(int)INFINITY
};

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitDataContainer(int num)
{
	if (num == 0)
	{
		currentScore = 0;
		highScore = 0;			//TODO:ハイスコアの読み込みを実装

		cntPowerUp = 0;
		shotLevel = 0;
		lockLevel = 0;
	}
}

/**************************************
スコア関連初期化処理
***************************************/
void InitScoreParameter(void)
{
	currentScore = 0;
	highScore = 0;				//TODO:ハイスコアの読み込みを追加
}

/**************************************
パワーアップ関連初期化処理
***************************************/
void InitPowerUpParameter(void)
{
	cntPowerUp = 0;
	shotLevel = 0;
	lockLevel = 0;
}

/**************************************
パワーアップ処理
***************************************/
void SetPowerUp(void)
{
	if (cntPowerUp >= DATACONTAINER_POWEUP_MAX)
		return;

	//パワーアップカウント追加
	cntPowerUp++;

	//ショットレベルかロックレベルをアップ
	if (cntPowerUp % 2 == 0)
		lockLevel++;
	else
		shotLevel++;

	//テロップ表示処理
	StartPowerUpTelopAnimation(cntPowerUp & 2);
}