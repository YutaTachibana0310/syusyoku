//=====================================
//
//データコンテナ処理[dataContainer.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "dataContainer.h"
#include "powerUpTelop.h"
#include "battleController.h"

/**************************************
マクロ定義
***************************************/
#define DATACONTAINER_POWEUP_MAX		(7)		//パワーアップ最大回数
#define DATACONTAINER_SHOTLEVEL_MAX		(4)		//ショットレベルマックス
#define DATACONTAINER_PlAYERHP_INIT		(100.0f)//HP初期値
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
static const int PowerUpBorder[DATACONTAINER_POWEUP_MAX] = {
	500000,
	1000000,
	1500000,
	2000000,
	2500000,
	3000000,
	(int)INFINITY
};

//各ロックオンレベルでの最大ロックオン数
static const int LockonMax[DATACONTAINER_LOCKLEVEL_MAX + 1] = {
	18, 36, 48, 54
};

//プレイヤーHP
static float playerHP;

/**************************************
プロトタイプ宣言
***************************************/

/**************************************
初期化処理
***************************************/
void InitDataContainer(int num)
{

	currentScore = 0;
	highScore = 0;			//TODO:ハイスコアの読み込みを実装

	cntPowerUp = 0;
	shotLevel = 0;
	lockLevel = 0;

	playerHP = DATACONTAINER_PlAYERHP_INIT;

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
HP初期化処理
***************************************/
void InitPlayerHP(void)
{
	playerHP = DATACONTAINER_PlAYERHP_INIT;
}

/**************************************
スコア加算処理
***************************************/
void AddScore(int addValue)
{
	//スコア加算処理
	currentScore += addValue;

	//パワーアップ判定処理
	if (currentScore >= PowerUpBorder[cntPowerUp])
		SetPowerUp();

	//エクステンド処理

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
	StartPowerUpTelopAnimation(cntPowerUp % 2);
}

/**************************************
ショットレベル取得処理
***************************************/
int GetShotLevel(void)
{
	return IsBonusTime() ? DATACONTAINER_SHOTLEVEL_MAX : 0;
}

/**************************************
ロックオンレベル取得処理
***************************************/
int GetLockonLevel(void)
{
	return lockLevel;
}

/**************************************
最大ロックオン取得処理
***************************************/
int GetLockonMax(void)
{
	return LockonMax[lockLevel];
}

/**************************************
スコア取得処理
***************************************/
int GetCurrentScore(void)
{
	return currentScore;
}

/**************************************
ハイスコア取得処理
***************************************/
int GetHighScore(void)
{
	return highScore;
}

/**************************************
HP取得処理
***************************************/
float GetPlayerHP(void)
{
	return playerHP;
}

/**************************************
HP増減取得処理
***************************************/
void AddPlayerHP(float value)
{
	playerHP += value;

	if (playerHP < 0.0f)
		playerHP = 0.0f;
}