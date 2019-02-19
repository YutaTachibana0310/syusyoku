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
#define STAGEDATA_MODEMAX	(3)
#define STAGEDATA_CONTAINERMAX	(STAGEDATA_MODEMAX*3)
/**************************************
構造体定義
***************************************/
typedef struct {
	STAGE_DATA *head;
	int current;
	int dataMax;
}CONTAINER_STAGEDATA;
/**************************************
グローバル変数
***************************************/
static CONTAINER_STAGEDATA container[STAGEDATA_CONTAINERMAX];
static CONTAINER_STAGEDATA bonusData;

//通常ステージデータのファイルパス
static const char* FilePath[STAGEDATA_CONTAINERMAX] = {
	"data/STAGE/StageDataFPS.dat",
	"data/STAGE/StageDataTop.dat",
	"data/STAGE/StageDataSide.dat",

	"data/STAGE/StageDataBonus.dat",
	"data/STAGE/StageDataTop.dat",
	"data/STAGE/StageDataSide.dat",

	"data/STAGE/StageDataFPSPresen.dat",
	"data/STAGE/StageDataTopPresen.dat",
	"data/STAGE/StageDataSidePresen.dat",
};

static bool flgBonusPresen;
static int headIndex;

/**************************************
プロトタイプ宣言
***************************************/
bool LoadStageData(int i);
void LoadBonusPresenData(void);

/**************************************
初期化処理
***************************************/
void InitStageData(int num)
{
	static bool initialized = false;
	for(int i = 0; i < STAGEDATA_CONTAINERMAX; i++)
		container[i].current = 0;

	if (!initialized)
	{
		for(int i = 0; i < STAGEDATA_CONTAINERMAX; i++)
			LoadStageData(i);
		LoadBonusPresenData();
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
		for(int i = -0; i < STAGEDATA_MODEMAX; i++)
			free(container[i].head);
	}
}

/**************************************
更新処理
***************************************/
int UpdateStageData(STAGE_DATA **out, DWORD currentFrame, int mode)
{
	int cntData = 0;
	CONTAINER_STAGEDATA *ptr = &container[STAGEDATA_MODEMAX * headIndex + mode];
	
	while (ptr->current < ptr->dataMax && ptr->head[ptr->current].emmittFrame == currentFrame)
	{
		if (cntData == 0)
			*out = &ptr->head[ptr->current];
		ptr->current++;
		cntData++;
	}


	return cntData;
}

/**************************************
ステージデータ読み込み処理
***************************************/
bool LoadStageData(int mode)
{
	FILE *fp = NULL;
	CONTAINER_STAGEDATA *entity = &container[mode];

	//ファイル読み込み
	fp = fopen(FilePath[mode], "r");
	if (fp == NULL)
		return false;

	//メモリ確保
	entity->dataMax = 0;
	int loadRes = fscanf(fp, "%d", &entity->dataMax);

	if (loadRes == EOF)
		return false;

	entity->head = (STAGE_DATA*)malloc(sizeof(STAGE_DATA) * entity->dataMax);
	ZeroMemory(entity->head, sizeof(STAGE_DATA) * entity->dataMax);

	//データ読み込み
	STAGE_DATA *ptr = entity->head;
	int sumEmmittFrame = 0;
	for (int i = 0; i < entity->dataMax; i++, ptr++)
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

/**************************************
ステージデータが終わっているかどうか
***************************************/
bool IsStageEnd(void)
{
	bool res = true;
	for (int i = 0; i < STAGEDATA_MODEMAX; i++)
	{
		if (container[i].current != container[i].dataMax)
		{
			res = false;
			break;
		}
}
	return res;
}

/**************************************
ボーナスタイムプレゼン用のデータ読み込み
***************************************/
void LoadBonusPresenData(void)
{
	CONTAINER_STAGEDATA *entity = &bonusData;

	FILE *fp = NULL;
	//ファイル読み込み
	fp = fopen("data/STAGE/StageDataBonus.dat", "r");
	if (fp == NULL)
		return;

	//メモリ確保
	entity->dataMax = 0;
	int loadRes = fscanf(fp, "%d", &entity->dataMax);

	if (loadRes == EOF)
		return;

	entity->head = (STAGE_DATA*)malloc(sizeof(STAGE_DATA) * entity->dataMax);
	ZeroMemory(entity->head, sizeof(STAGE_DATA) * entity->dataMax);

	//データ読み込み
	STAGE_DATA *ptr = entity->head;
	int sumEmmittFrame = 0;
	for (int i = 0; i < entity->dataMax; i++, ptr++)
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
}

/**************************************
ボーナスタイムプレゼンセット
***************************************/
void SetBonusPresenFlag(bool state)
{
	flgBonusPresen = state;
}

/**************************************
ステージコンテナヘッド設定処理
***************************************/
void SetStageDataContainerHead(int n)
{
	headIndex = n;
}