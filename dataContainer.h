//=====================================
//
//データコンテナヘッダ[dataContainer.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _DATACONTAINER_H_
#define _DATACONTAINER_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define DATACONTAINER_SHOTLEVEL_MAX		(4)		//ショットレベル最大値
#define DATACONTAINER_LOCKLEVEL_MAX		(3)		//ロックオンレベル最大値
#define DATACONTAINER_PLAYERNAME_MAX	(5)		//プレイヤー名最大文字数
#define DATACONTAINER_HIGHSCORE_MAX		(10)	//ハイスコアの保存数
/**************************************
構造体定義
***************************************/
typedef struct {
	int score;
	char playerName[DATACONTAINER_PLAYERNAME_MAX];
}DATA_HIGHSCRE;

/**************************************
プロトタイプ宣言
***************************************/
void InitDataContainer(int num);

void AddScore(int addValue);
void SetPowerUp(void);
void InitPlayerHP(void);

int GetShotLevel(void);
int GetLockonLevel(void);
int GetCurrentScore(void);
DATA_HIGHSCRE* GetHighScore(void);
int GetLockonMax(void);

float GetPlayerHP(void);
void AddPlayerHP(float value);

bool LoadHighScoreData(void);
bool SaveHighScoreData(void);

#endif