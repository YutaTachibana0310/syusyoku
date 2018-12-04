//=====================================
//
//ミドルエネミーモデルヘッダ[middleEnemyModel.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _MIDDLEENMYMODEL_H_
#define _MIDDLEENMYMODEL_H_

#include "main.h"

/**************************************
マクロ定義
***************************************/
#define MIDDLEENEMY_MAX			(36)
/**************************************
構造体定義
***************************************/
//ミドルエネミー状態定義
enum MIDDLEENEMY_STATE
{
	MiddleEnemyMove,		//目的座標に移動する状態
	MiddleEnemyAttack,		//攻撃状態
	MiddleEnemyStateMax
};

//ミドルエネミー構造体
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

}MIDDLEENEMYMODEL;
/**************************************
プロトタイプ宣言
***************************************/
void InitMiddleEnemyModel(int num);
void UninitMiddleEnemyModel(int num);
void UpdateMiddleEnemyModel(void);
void DrawMiddleEnemyModel(void);
void ChangeStateMiddleEnemy(MIDDLEENEMYMODEL *ptr, int next);
MIDDLEENEMYMODEL *GetMiddleEnemyAdr(int num);
void LockonMiddleEnemy(void);

void UpdateMiddleEnemyMove(MIDDLEENEMYMODEL *enemy);
void UpdateMiddleEnemyAttack(MIDDLEENEMYMODEL *enemy);

void EnterMiddleEnemyMove(MIDDLEENEMYMODEL *enemy);
void EnterMiddleEnemyAttack(MIDDLEENEMYMODEL *enemy);

void ExitMiddleEnemyMove(MIDDLEENEMYMODEL *enemy);
void ExitMiddleEnemyAttack(MIDDLEENEMYMODEL *enemy);

#endif