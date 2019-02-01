//=====================================
//
//データコンテナ処理[dataContainer.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "dataContainer.h"
#include "powerUpTelop.h"
#include "battleController.h"
#include <stdio.h>
#include <string.h>

/**************************************
マクロ定義
***************************************/
#define DATACONTAINER_POWEUP_MAX		(7)			//パワーアップ最大回数
#define DATACONTAINER_SHOTLEVEL_MAX		(4)			//ショットレベルマックス
#define DATACONTAINER_PlAYERHP_INIT		(100.0f)	//HP初期値
#define DATACONTAINER_SAVEDATA_PATH		"data/SETTINGS/data.ini"	//ハイスコアデータのファイルパス	

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
//スコア関連パラメータ
static int currentScore;
static DATA_HIGHSCRE highScore[DATACONTAINER_HIGHSCORE_MAX];

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
void UpdateRanking(int index);

/**************************************
初期化処理
***************************************/
void InitDataContainer(int num)
{

	currentScore = 55555;
	LoadHighScoreData();

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
	//highScore = 0;				//TODO:ハイスコアの読み込みを追加
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
ランキング更新確認処理
***************************************/
bool CheckUpdateRanking(int *index)
{
	for (int i = 0; i < DATACONTAINER_HIGHSCORE_MAX; i++)
	{
		if (currentScore > highScore[i].score)
		{
			UpdateRanking(i);
			*index = i;
			return true;
		}
	}

	return false;
}

/**************************************
ランキング更新確認処理
***************************************/
void UpdateRanking(int index)
{
	memcpy(&highScore[index + 1], &highScore[index], sizeof(DATA_HIGHSCRE)*(DATACONTAINER_HIGHSCORE_MAX - 1 - index));
	highScore[index].score = currentScore;
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
DATA_HIGHSCRE* GetHighScore(int id)
{
	return &highScore[id];
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

/**************************************
ハイスコアデータ読み込み処理
***************************************/
bool LoadHighScoreData(void)
{
	FILE *fp = NULL;
	fp = fopen(DATACONTAINER_SAVEDATA_PATH, "rb");

	//読み込みに失敗したのでハイスコアを初期化してリターン
	if (fp == NULL)
	{
		for (int i = 0; i < DATACONTAINER_HIGHSCORE_MAX; i++)
		{
			highScore[DATACONTAINER_HIGHSCORE_MAX - i - 1].score = (i + 1) * 10000;
			highScore[DATACONTAINER_HIGHSCORE_MAX - i - 1].playerName[0] = 7;
			highScore[DATACONTAINER_HIGHSCORE_MAX - i - 1].playerName[1] = 0;
			highScore[DATACONTAINER_HIGHSCORE_MAX - i - 1].playerName[2] = 11;
			highScore[DATACONTAINER_HIGHSCORE_MAX - i - 1].playerName[3] = 26;
			highScore[DATACONTAINER_HIGHSCORE_MAX - i - 1].playerName[4] = 26;
		}
		return false;
	}

	fread(&highScore, sizeof(DATA_HIGHSCRE), DATACONTAINER_HIGHSCORE_MAX, fp);
	fclose(fp);
	return true;
}

/**************************************
ハイスコアデータ保存処理
***************************************/
bool SaveHighScoreData(void)
{
	FILE *fp = NULL;
	fp = fopen(DATACONTAINER_SAVEDATA_PATH, "wb");

	//読み込み失敗
	if (fp == NULL)
	{
		return false;
	}

	fwrite(highScore, sizeof(DATA_HIGHSCRE), DATACONTAINER_HIGHSCORE_MAX, fp);
	fclose(fp);
	return true;
}