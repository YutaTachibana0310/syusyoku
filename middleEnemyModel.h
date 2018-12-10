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

}MIDDLEENEMY;
/**************************************
プロトタイプ宣言
***************************************/
void InitMiddleEnemy(int num);
void UninitMiddleEnemy(int num);
void UpdateMiddleEnemy(void);
void DrawMiddleEnemy(void);
void ChangeStateMiddleEnemy(MIDDLEENEMY *ptr, int next);
MIDDLEENEMY *GetMiddleEnemyAdr(int num);
void LockonMiddleEnemy(void);
void CollisionMiddleEnemyAndBullet(void);

//各状態更新処理
void UpdateMiddleEnemyMove(MIDDLEENEMY *enemy);
void UpdateMiddleEnemyAttack(MIDDLEENEMY *enemy);
void UpdateMiddleEnemyEscape(MIDDLEENEMY *enemy);
void UpdateMiddleEnemyWait(MIDDLEENEMY *enemy);

//各状態入場処理
void EnterMiddleEnemyMove(MIDDLEENEMY *enemy);
void EnterMiddleEnemyAttack(MIDDLEENEMY *enemy);
void EnterMiddleEnemyEscape(MIDDLEENEMY *enemy);
void EnterMiddleEnemyWait(MIDDLEENEMY *enemy);

//各状態退場処理
void ExitMiddleEnemyMove(MIDDLEENEMY *enemy);
void ExitMiddleEnemyAttack(MIDDLEENEMY *enemy);
void ExitMiddleEnemyEscape(MIDDLEENEMY *enemy);
void ExitMiddleEnemyWait(MIDDLEENEMY *enemy);

#endif