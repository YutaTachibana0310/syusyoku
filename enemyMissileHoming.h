//===================================
//
//エネミーミサイル発射ヘッダ[enemyMissileHoming.h]
//Author:GP11A341 21 立花雄太
//
//===================================
#ifndef _ENEMYMISSILEHOMING_H_
#define _ENEMYMISSILEHOMING_H_

#include "enemyMissile.h"

/************************************
マクロ定義
*************************************/

/************************************
構造体定義
*************************************/

/************************************
プロトタイプ宣言
*************************************/
void EnemyMissileHomingEnter(ENEMYMISSILE *ptr);
void EnemyMissileHomingUpdate(ENEMYMISSILE *ptr);
void EnemyMissileHomingExit(ENEMYMISSILE *ptr);

#endif
