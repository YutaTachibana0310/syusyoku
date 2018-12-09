//=====================================
//
//テンプレートヘッダ[template.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SMALLENEMYMODEL_H_
#define _SMALLENEMYMODEL_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define SMALLENEMY_MAX		(128)

/**************************************
構造体定義
***************************************/
enum SMALLENEMY_STATE
{
	SmallEnemyMove,
	SmallEnemyAttack,
	SmallEnemyEscape,
	SmallEnemyStateMax
};

typedef struct
{
	bool active;

	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int cntFrame;
	int state;

	D3DXVECTOR3 startPos, goalPos;
	D3DXVECTOR3 startRot, goalRot;

	float hp;
	int atkNum;

}SMALLENEMYMODEL;

/**************************************
プロトタイプ宣言
***************************************/
void InitSmallEnemyModel(int num);
void UninitSmallEnemyModel(int num);
void UpdateSmallEnemyModel(void);
void DrawSmallEnemyModel(void);
void SetSmallEnemyModel(D3DXVECTOR3 pos);

//更新処理テーブル
void UpdateSmallEnemyMove(SMALLENEMYMODEL *enemy);
void UpdateSmallEnemyAttack(SMALLENEMYMODEL *enemy);
void UpdateSmallEnemyEscape(SMALLENEMYMODEL *enemy);

//入場処理テーブル
void EnterSmallEnemyMove(SMALLENEMYMODEL *enemy);
void EnterSmallEnemyAttack(SMALLENEMYMODEL *enemy);
void EnterSmallEnemyEscape(SMALLENEMYMODEL *enemy);

//退場処理テーブル
void ExitSmallEnemyMove(SMALLENEMYMODEL *enemy);
void ExitSmallEnemyAttack(SMALLENEMYMODEL *enemy);
void ExitSmallEnemyEscape(SMALLENEMYMODEL *enemy);

#endif