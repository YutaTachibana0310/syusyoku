//=====================================
//
//ステージデータ処理[stageData.cpp]
//Author:GP11A341 21 立花雄太
//
//=====================================
#include "stageData.h"
#include <stdio.h>


/**************************************
マクロ定義
***************************************/
#define STAGEDATA_PATH	"data/STAGE/StageData.dat"

/**************************************
構造体定義
***************************************/

/**************************************
グローバル変数
***************************************/
static STAGE_DATA *dataHead;
static int currentIndex;
static int dataMax;

/**************************************
プロトタイプ宣言
***************************************/
bool LoadStageData(void);

/**************************************
初期化処理
***************************************/
void InitStageData(int num)
{
	static bool initialized = false;
	currentIndex = 0;

	if (!initialized)
	{
		LoadStageData();
		initialized = true;
	}
}

/**************************************
終了処理
***************************************/
void UninitStageData(int num)
{
	if (num == 0)
	{
		free(dataHead);
	}
}

/**************************************
更新処理
***************************************/
int UpdateStageData(STAGE_DATA **out, DWORD currentFrame)
{
	int cntData = 0;
	while (currentIndex < dataMax && dataHead[currentIndex].emmittFrame == currentFrame)
	{
		if (cntData == 0)
			*out = &dataHead[currentIndex];
		currentIndex++;
		cntData++;
	}


	return cntData;
}

/**************************************
ステージデータ読み込み処理
***************************************/
bool LoadStageData(void)
{
	FILE *fp = NULL;

	//ファイル読み込み
	fp = fopen(STAGEDATA_PATH, "r");
	if (fp == NULL)
		return false;

	//メモリ確保
	dataMax = 0;
	int loadRes = fscanf(fp, "%d", &dataMax);

	if (loadRes == EOF)
		return false;

	dataHead = (STAGE_DATA*)malloc(sizeof(STAGE_DATA) * dataMax);
	ZeroMemory(dataHead, sizeof(STAGE_DATA) * dataMax);

	//データ読み込み
	STAGE_DATA *ptr = dataHead;
	int sumEmmittFrame = 0;
	for (int i = 0; i < dataMax; i++, ptr++)
	{
		fscanf(fp, "%d,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f",
			&ptr->emmittFrame,
			&ptr->type,
			&ptr->initPos.x,
			&ptr->initPos.y,
			&ptr->initPos.z,
			&ptr->targetPos.x,
			&ptr->targetPos.y,
			&ptr->targetPos.z,
			&ptr->controller1.x,
			&ptr->controller1.y,
			&ptr->controller1.z,
			&ptr->controller2.x,
			&ptr->controller2.y,
			&ptr->controller2.z);

		ptr->emmittFrame += sumEmmittFrame;
		sumEmmittFrame = ptr->emmittFrame;
	}

	fclose(fp);
	return true;
}