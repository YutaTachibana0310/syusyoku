//============================================
//
//エネミーミサイルヘッダ[enemyMissile.h]
//AUthor:GP11A341 21 立花雄太
//
//============================================
#ifndef _ENEMYMISSILE_H_
#define _ENEMYMISSILE_H_

#include "main.h"
#include "collider.h"

/**********************************************
マクロ定義
**********************************************/
#define ENEMYMISSILE_MAX			(512)				//ミサイル最大数

/**********************************************
構造体定義
**********************************************/
typedef struct
{
	bool active;					//使用フラグ
	D3DXVECTOR3 pos;				//座標
	D3DXQUATERNION rot;				//回転クォータニオン
	D3DXVECTOR3 velocity;			//移動方向
	D3DXVECTOR3 targetPos;			//目標座標
	COLLIDER_SPHERE collider;				//当たり判定

	float speed;					//移動スピード
	int cntFrame;					//フレームカウント
	

	int state;						//状態
	int nextState;					//次の状態

	int period;
	float hp;
}ENEMYMISSILE;

/********************************************
列挙体定義
*********************************************/
enum ENEMYMISSILE_STATE
{
	ENEMYMISSILE_LAUNCH,			//発射直後状態
	ENEMYMISSILE_HOMING,			//ホーミング状態
	ENEMYMISSILE_STRAIGHT,			//直進状態
	ENEMYMISSILE_STATEMAX
};

/**********************************************
プロトタイプ宣言
**********************************************/
void InitEnemyMissile(int num);
void UninitEnemyMissile(int num);
void UpdateEnemyMissile(void);
void DrawEnemyMissile(void);
ENEMYMISSILE *GetEnemyMissileAdr(int n);
void ChangeStateEnemyMissile(ENEMYMISSILE *ptr, int targetState);
void SetEnemyMissile(D3DXVECTOR3 pos, D3DXVECTOR3 moveDir, D3DXVECTOR3 targetPos);
void CollisionEnemyMissileAndBullet(void);
void LockonEnemyMissile(void);
#endif