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

/**********************************************
構造体定義
**********************************************/
typedef struct
{
	bool active;					//使用フラグ
	D3DXVECTOR3 pos;				//座標
	D3DXQUATERNION rot;				//回転クォータニオン
	D3DXVECTOR3 moveDir;			//移動方向
	D3DXVECTOR3 targetPos;			//目標座標
	float speed;					//移動スピード
	SPHERE collider;				//当たり判定
	bool flgHoming;					//ホーミングフラグ
	int cntFrame;					//フレームカウント
}ENEMYMISSILE;
/**********************************************
プロトタイプ宣言
**********************************************/
void InitEnemyMissile(int num);
void UninitEnemyMissile(void);
void UpdateEnemyMissile(void);
void DrawEnemyMissile(void);
ENEMYMISSILE *GetEnemyMissileAdr(int n);

#endif