//===================================
//
//エネミーミサイル発射ヘッダ[enemyMissileStraight.h]
//Author:GP11A341 21 立花雄太
//
//===================================
#ifndef _ENEMYMISSILESTRAIGHT_H_
#define _ENEMYMISSILESTRAIGHT_H_

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
void EnemyMissileStraightEnter(ENEMYMISSILE *ptr);
void EnemyMissileStraightUpdate(ENEMYMISSILE *ptr);
void EnemyMissileStraightExit(ENEMYMISSILE *ptr);

#endif
