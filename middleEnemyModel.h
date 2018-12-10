//=====================================
//
//ミドルエネミーモデルヘッダ[middleEnemyModel.h]
//Author:GP11A341 21 立花雄太
//
//=====================================
#ifndef _MIDDLEENMYMODEL_H_
#define _MIDDLEENMYMODEL_H_

#include "main.h"
#include "collider.h"

/**************************************
マクロ定義
***************************************/
#define MIDDLEENEMY_MAX				(36)
#define MIDDLEENEMY_COLLIDER_RADIUS	(20.0f)

/**************************************
構造体定義
***************************************/
//ミドルエネミー状態定義
enum MIDDLEENEMY_STATE
{
	MiddleEnemyMove,		//目的座標に移動する状態
	MiddleEnemyAttack,		//攻撃状態
	MiddleEnemyEscape,		//逃走状態
	MiddleEnemyWait,		//待機状態
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

	SPHERE collider;

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
void CollisionMiddleEnemyAndBullet(void);

//各状態更新処理
void UpdateMiddleEnemyMove(MIDDLEENEMYMODEL *enemy);
void UpdateMiddleEnemyAttack(MIDDLEENEMYMODEL *enemy);
void UpdateMiddleEnemyEscape(MIDDLEENEMYMODEL *enemy);
void UpdateMiddleEnemyWait(MIDDLEENEMYMODEL *enemy);

//各状態入場処理
void EnterMiddleEnemyMove(MIDDLEENEMYMODEL *enemy);
void EnterMiddleEnemyAttack(MIDDLEENEMYMODEL *enemy);
void EnterMiddleEnemyEscape(MIDDLEENEMYMODEL *enemy);
void EnterMiddleEnemyWait(MIDDLEENEMYMODEL *enemy);

//各状態退場処理
void ExitMiddleEnemyMove(MIDDLEENEMYMODEL *enemy);
void ExitMiddleEnemyAttack(MIDDLEENEMYMODEL *enemy);
void ExitMiddleEnemyEscape(MIDDLEENEMYMODEL *enemy);
void ExitMiddleEnemyWait(MIDDLEENEMYMODEL *enemy);

#endif