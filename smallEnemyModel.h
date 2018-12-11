//=====================================
//
//スモールエネミーヘッダ[smallEnemyModel.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _SMALLENEMY_H_
#define _SMALLENEMY_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/
#define SMALLENEMY_MAX			(128)

/**************************************
構造体定義
***************************************/
//スモールエネミー状態定義
enum SMALLENEMY_STATE
{
	SmallEnemyMove,			//移動状態
	SmallEnemyAttack,		//攻撃状態
	SmallEnemyEscape,		//逃走状態
	SmallEnemyWait,			//待機状態
	SmallEnemyStateMax		
};

//スモールエネミー構造体
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

	SPHERE collider;
}SMALLENEMY;
/**************************************
プロトタイプ宣言
***************************************/
void InitSmallEnemy(int num);
void UninitSmallEnemy(int num);
void UpdateSmallEnemy(void);
void DrawSmallEnemy(void);
void ChangeStateSmallEnemy(SMALLENEMY *ptr, int next);
SMALLENEMY *GetSmallEnemyAdr(int id);
void LockonSmallEnemy(void);
void CollisionSmallEnemyAndBullet(void);

//各状態更新処理
void UpdateSmallEnemyMove(SMALLENEMY *enemy);
void UpdateSmallEnemyAttack(SMALLENEMY *enemy);
void UpdateSmallEnemyEscape(SMALLENEMY *enemy);
void UpdateSmallEnemyWait(SMALLENEMY *enemy);

//各状態入場処理
void EnterSmallEnemyMove(SMALLENEMY *enemy);
void EnterSmallEnemyAttack(SMALLENEMY *enemy);
void EnterSmallEnemyEscape(SMALLENEMY *enemy);
void EnterSmallEnemyWait(SMALLENEMY *enemy);

//各状態退場処理
void ExitSmallEnemyMove(SMALLENEMY *enemy);
void ExitSmallEnemyAttack(SMALLENEMY *enemy);
void ExitSmallEnemyEscape(SMALLENEMY *enemy);
void ExitSmallEnemyWait(SMALLENEMY *enemy);

#endif